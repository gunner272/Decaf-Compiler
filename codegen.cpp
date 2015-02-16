#include "codegen.h"
//#include "parse.tab.hpp"

int validity;
static IRBuilder<> Builder(getGlobalContext());

void CodeGenContext::generateCode(ASTProgram* root)
{
	std::cout << "Generating code...\n";

	vector<const Type*> argTypes;
	FunctionType *ftype = FunctionType::get(Type::getVoidTy(getGlobalContext()), argTypes, false);
	mainFunction = Function::Create(ftype, GlobalValue::InternalLinkage, "Class Program", module);
	BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", mainFunction, 0);

	pushBlock(bblock);
	*root->codeGen(*this); /* emit bytecode for the toplevel block */
	popBlock();

	if(validity == 0)
	{
		std::cout << "Code is generated.\n";
		PassManager pm;
		pm.add(createPrintModulePass(&outs()));
		pm.run(*module);
	}

}

static const Type *typeOf(string type) 
{
	if (type.compare("int") == 0) {
		return Type::getInt64Ty(getGlobalContext());
	}
	else if (type.compare("boolean")==0)
		return Type::getInt1Ty(getGlobalContext());

	return Type::getVoidTy(getGlobalContext());
}


Value *ASTProgram::codeGen(CodeGenContext& context)
{
	int i;
	cout<<"Program Created"<<endl;
	Value *last = NULL;
	for(i=0;i<fdl_.size();i++)
	{
		last = fdl_[i].codeGen(context);	
	}

	for(i=0;i<mtl_.size();i++)
	{
		last = mtl_[i].codeGen(context);
	}
}

Value *ASTFieldDecl::codeGen(CodeGenContext& context)
{
	int i;
	Value *v;
	for(i=0;i<var.size();i++)
	{
		AllocaInst *alloc = new AllocaInst(typeOf(type_), var[i].id_, context.currentBlock());
		context.locals()[var[i].id_] = alloc;
		v=alloc;
	}
	return v;
}

Value *ASTVariable::codeGen(CodeGenContext& context)
{
}




Value *ASTAssignmentStmt::codeGen(CodeGenContext& context)
{
	cout<<"Assignment Statement of :"<<lval->id_<<endl;
	if(context.locals().find(lval->id_) == context.locals().end())
	{
		cerr<<"Undeclared Variable in assignment statement"<<lval->id_<<endl;
		validity = 1;
		return NULL;
	}
	Value *st = e_.codeGen(context);

	if(st!=NULL)
		st = new StoreInst(st, context.locals()[lval->id_], context.currentBlock());

	return st;
}



Value *ASTReturnStmt::codeGen(CodeGenContext& context)
{
	if(flag==1)
		return ReturnInst::Create(getGlobalContext(),context.currentBlock());	

	else
	{
		Value *val=e_->codeGen(context);
		return ReturnInst::Create(getGlobalContext(),val,context.currentBlock());	
	}
}


Value *ASTAssign::codeGen(CodeGenContext& context)
{

	Value *st = e_.codeGen(context);

	if(st!=NULL)
		st = new StoreInst(st, context.locals()[id_], context.currentBlock());

	return st;

}


Value *ASTForStmt::codeGen(CodeGenContext& context)
{
	Value *StartVal = ass.codeGen(context);
	if (StartVal == NULL) return NULL;

	Function *TheFunction = Builder.GetInsertBlock()->getParent();
	BasicBlock *PreheaderBB = Builder.GetInsertBlock();
	BasicBlock *LoopBB = BasicBlock::Create(getGlobalContext(), "loop", TheFunction);
	cout<<"For started"<<endl;

	Builder.CreateBr(LoopBB);

	Builder.SetInsertPoint(LoopBB);

//	PHINode *Variable = Builder.CreatePHI(Type::getDoubleTy(getGlobalContext()), 2, VarName.c_str());
//	Variable->addIncoming(StartVal, PreheaderBB);

//	Value *OldVal = NamedValues[VarName];
//	NamedValues[VarName] = Variable;
	if (bl.codeGen(context) == NULL)
		return NULL;


	Value *EndCond = e_.codeGen(context);
	if (EndCond == NULL) return EndCond;

	EndCond = Builder.CreateFCmpONE(EndCond, 
			ConstantFP::get(getGlobalContext(), APFloat(0.0)),
			"loopcond");

	BasicBlock *LoopEndBB = Builder.GetInsertBlock();
	BasicBlock *AfterBB = BasicBlock::Create(getGlobalContext(), "afterloop", TheFunction);

	Builder.CreateCondBr(EndCond, LoopBB, AfterBB);

	Builder.SetInsertPoint(AfterBB);

//	Variable->addIncoming(NextVar, LoopEndBB);

	return NULL;
}


Value *ASTIFStmt::codeGen(CodeGenContext& context)
{
	Value *CondV = e_.codeGen(context);
	if(CondV!=NULL)
	{
		CondV = Builder.CreateFCmpONE(CondV,ConstantFP::get(getGlobalContext(),APFloat(0.0)),"ifcond");
		Function *TheFunction = Builder.GetInsertBlock()->getParent();

		BasicBlock *IfBB = BasicBlock::Create(getGlobalContext(), "if", TheFunction);
		BasicBlock *ElseBB = BasicBlock::Create(getGlobalContext(), "else");
		BasicBlock *MergeBB = BasicBlock::Create(getGlobalContext(), "ifcont");


		Builder.CreateCondBr(CondV, IfBB, ElseBB);
		Builder.SetInsertPoint(IfBB);
		cout<<"Statement number: "<<ifb.stmts.size()<<endl;
		Value *IfV = ifb.codeGen(context);
		Builder.CreateBr(MergeBB);

		IfBB = Builder.GetInsertBlock();
		TheFunction->getBasicBlockList().push_back(ElseBB);
		Builder.SetInsertPoint(ElseBB);
		Value *ElseV = elb.codeGen(context);
		if(ElseV == NULL)return NULL;
		Builder.CreateBr(MergeBB);
		ElseBB = Builder.GetInsertBlock();
		TheFunction->getBasicBlockList().push_back(MergeBB);
		Builder.SetInsertPoint(MergeBB);
		cout<<"If Statement"<<endl;
		//PHINode *PN = Builder.CreatePHI(Type::getDoubleTy(getGlobalContext()),"iftmp");

	//	PN->addIncoming(IfV, IfBB);
	//	PN->addIncoming(ElseV, ElseBB);
		return NULL;
	}
	
	return NULL;
}





Value *ASTStatement::codeGen(CodeGenContext& context)
{
	
	if(flag == 0)
	{
		return 	ass->codeGen(context);
	}

	if(flag==1)
	{
		return rt->codeGen(context);
	}



	if(flag == 3)
	{
	BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "block");
	context.pushBlock(bblock);
	*blk->codeGen(context);
	context.popBlock();
	}

	if(flag==4)
	{
		return fr->codeGen(context);
	}
	
	if(flag==5)
	{
		return ifel->codeGen(context);
	}

	if(flag==7)
	{
		return mc->codeGen(context);
	}
	return NULL;
}

Value *ASTNormalMethodCall::codeGen(CodeGenContext& context)
{
	Function *function = context.module->getFunction(id_);
	if(function==NULL)
	{
		cerr<<id_<<"function not declared"<<endl;
		validity = 1;
		return NULL;
	}
	int i;
	vector<Value*> args;

	for(i=0;i<el_.size();i++)
	{
		args.push_back(el_[i].codeGen(context));
	}

	CallInst *call = NULL;
	call = CallInst::Create(function,args.begin(),args.end(),"",context.currentBlock());


	return call;
}

Value *ASTMethodCall::codeGen(CodeGenContext& context)
{
	if(flag==0)
	{
		Value * call = nm->codeGen(context);
		return call;
	}
}

Value *ASTBinexpr::codeGen(CodeGenContext& context)
{
	Instruction::BinaryOps instr;
	if(op_.compare("+")==0)
	{
		instr=Instruction::Add;
	}

	else if(op_.compare("-")==0)
	{
		instr=Instruction::Sub;
	}

	else if(op_.compare("*")==0)
	{
		instr=Instruction::Mul;
	}

	else if(op_.compare("/")==0)
	{
		instr=Instruction::SDiv;
	}
	
	else if(op_.compare("%")==0)
	{
		instr=Instruction::SRem;
	}
	
	else if(op_.compare("&&")==0)
	{
		instr=Instruction::And;
	}
	
	else if(op_.compare("||")==0)
	{
		instr=Instruction::Or;
	}

	else if(op_.compare("||")==0)
	{
		instr=Instruction::Or;
	}

	else if(op_.compare("==")==0)
	{
		return CmpInst::Create(Instruction::ICmp, ICmpInst::ICMP_EQ, lhs.codeGen(context), rhs.codeGen(context),"",context.currentBlock());
	}
	
	else if(op_.compare("<=")==0)
	{
		return CmpInst::Create(Instruction::ICmp, ICmpInst::ICMP_ULE, lhs.codeGen(context), rhs.codeGen(context),"",context.currentBlock());
	}

	else if(op_.compare(">=")==0)
	{
		return CmpInst::Create(Instruction::ICmp, ICmpInst::ICMP_UGE, lhs.codeGen(context), rhs.codeGen(context),"",context.currentBlock());
	}
	
	else if(op_.compare("<")==0)
	{
		return CmpInst::Create(Instruction::ICmp, ICmpInst::ICMP_ULT, lhs.codeGen(context), rhs.codeGen(context),"",context.currentBlock());
	}

	else if(op_.compare(">")==0)
	{
		return CmpInst::Create(Instruction::ICmp, ICmpInst::ICMP_UGT, lhs.codeGen(context), rhs.codeGen(context),"",context.currentBlock());
	}

	else if(op_.compare("!=")==0)
	{
		return CmpInst::Create(Instruction::ICmp, ICmpInst::ICMP_NE, lhs.codeGen(context), rhs.codeGen(context),"",context.currentBlock());
	}

	Value *bin=NULL;

	Value *l = lhs.codeGen(context);
	Value *r = rhs.codeGen(context);

	if(l!=NULL && r!=NULL)
		bin = BinaryOperator::Create(instr, lhs.codeGen(context),rhs.codeGen(context), "", context.currentBlock());

	return bin;

}


Value *ASTUnexpr::codeGen(CodeGenContext& context)
{
}

Value *ASTLiteral::codeGen(CodeGenContext& context)
{
	char s[1000006];
	strcpy(s,val.c_str());
	int value = atoi(s);

	return ConstantInt::get(Type::getInt64Ty(getGlobalContext()),value,true);
}



Value *ASTExpr::codeGen(CodeGenContext& context)
{
	if(flag==0)
	{
		return m->codeGen(context);
	}


	if(flag == 1)
	{
		return be->codeGen(context);		
	}


	if(flag==2)
	{
	if (context.locals().find(lc->id_) == context.locals().end()) 
	{
		std::cerr << "undeclared variable in expression " << lc->id_ << std::endl;
		validity = 1;
		return NULL;
	}
	return new LoadInst(context.locals()[lc->id_], "", false, context.currentBlock());
	}

	if(flag==4)
	{
		return ue->codeGen(context);
	}

	if(flag==3)
	{
		return li->codeGen(context);
	}
	return NULL;
}


Value *ASTBlock::codeGen(CodeGenContext& context)
{
	int i;

	for(i=0;i<v_sub.size();i++)
	{
		v_sub[i].codeGen(context);
	}
	for(i=0;i<stmts.size();i++)
	{
		stmts[i].codeGen(context);
	}
}


Value *ASTMetDecl::codeGen(CodeGenContext& context)
{
	cout<<"New Function"<<endl;
	int i;
	vector<const Type*> argTypes;
	for(i=0;i<var_decls.size();i++)
	{
		argTypes.push_back(typeOf(var_decls[i].type_id));
	}

	FunctionType *ftype = FunctionType::get(typeOf(ret_type),argTypes,false);
	Function *function = Function::Create(ftype,GlobalValue::InternalLinkage,id_,context.module);
	BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", function, 0);
	Builder.SetInsertPoint(bblock);
	context.pushBlock(bblock);
	for(i=0;i<var_decls.size();i++)
	{
		var_decls[i].codeGen(context);
	}
	*bl->codeGen(context);
	ReturnInst::Create(getGlobalContext(),bblock);	
	context.popBlock();
	return function;
}


Value *ASTVarsSub::codeGen(CodeGenContext& context)
{
	int i;
	for(i=0;i<vars.size();i++)
	{
	cout<<"declaring "<<vars[i]<<endl;
	AllocaInst *alloc = new AllocaInst(typeOf(type_id), vars[i], context.currentBlock());
	context.locals()[vars[i]] = alloc;
	}
}



