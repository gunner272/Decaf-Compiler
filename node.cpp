#include<bits/stdc++.h>
#include<llvm/Value.h>
using namespace std;

class CodeGenContext;

class ASTNode
{
};

class ASTProgram;
class ASTMetDecl;
class ASTLocation;
class ASTStatement;
class ASTVarsSub;
class ASTExpr;
class ASTMethodCall;
class ASTNormalMethodCall;
class ASTCalloutMethodCall;
class ASTBinexpr;
class ASTUnexpr;
class ASTLiteral;
class ASTAssignmentStmt;
class ASTReturnStmt;
class ASTContStmt;
class ASTBlock;
class ASTForStmt;
class ASTIFStmt;
class ASTBreakStmt;
class ASTMethodCall;
class ASTFieldDecl;
class ASTIntLiteral;
class ASTVariable;



class ASTProgram : public ASTNode
{
    public:
        vector<ASTMetDecl> mtl_;
        vector<ASTFieldDecl> fdl_;

        ASTProgram(vector<ASTFieldDecl> f,vector<ASTMetDecl> m)
        {
            mtl_=m;
            fdl_=f;
        }
        
        ASTProgram(vector<ASTFieldDecl> f)
        {
            fdl_ = f;
        }
        
        ASTProgram(){}
        
        ASTProgram(vector<ASTMetDecl> m)
        {
                mtl_=m;
        }

	virtual llvm::Value* codeGen(CodeGenContext& context);
};



class ASTVariable : public ASTNode
{
    public:
        string id_;
        ASTIntLiteral *lit_;
        int flag;
        ASTVariable(string id, ASTIntLiteral &lit)
        {
            id_=id;
            lit_=&lit;
            flag = 0;
        }

        ASTVariable(string id)
        {
            id_=id;
            flag=1;
        }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};




class ASTFieldDecl : public ASTNode
{
    public:
        string type_;
        vector<ASTVariable> var;

        ASTFieldDecl(string type,vector<ASTVariable> ivar)
        {
            type_=type;
            var = ivar;    
        }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};


class ASTStatement : public ASTNode
{
    
    public:
        int flag;
        ASTAssignmentStmt *ass;
        ASTReturnStmt *rt;
        ASTContStmt *cn;
        ASTBlock *blk;
        ASTForStmt *fr;
        ASTIFStmt *ifel;
        ASTBreakStmt *brk;
        ASTMethodCall *mc;

        ASTStatement(){}

        ASTStatement(ASTAssignmentStmt &iass)
        {
            ass = &iass;
            flag=0;
        }

        ASTStatement(ASTReturnStmt &irt)
        {
            rt=&irt;
            flag=1 ;
        }
    
        ASTStatement(ASTContStmt &icn )
        {
            cn = &icn ;
            flag=2 ;
        }

        ASTStatement(ASTBlock &iblk)
        {

            blk = &iblk ;
            flag= 3;
        }
        ASTStatement(ASTForStmt &ifr)
        {

            fr = &ifr;
            flag=4 ;
        }
        ASTStatement(ASTIFStmt &iifel)
        {

            ifel = &iifel;
            flag=5 ;
        }
        ASTStatement(ASTBreakStmt &ibrk)
        {

            brk = &ibrk;
            flag = 6;
        }
        ASTStatement(ASTMethodCall &imc)
        {
            mc=&imc;
            flag=7;
        }

	virtual llvm::Value* codeGen(CodeGenContext& context);

};


class ASTBlock : public ASTStatement
{
    public:
        int flag ;
        vector<ASTVarsSub> v_sub;
        vector<ASTStatement> stmts;
        ASTBlock()
        {
            flag = -1;
        }
        ASTBlock(vector<ASTVarsSub> vs, vector<ASTStatement> st)
        {
            v_sub = vs;
            stmts = st;
            flag = 0 ;
        }
        ASTBlock(vector<ASTStatement>st)
        {
            stmts = st;
            flag = 1;
        }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};




class ASTMetDecl : public ASTNode
{
    public:
        string ret_type;
        string id_;
        ASTBlock *bl;
        vector<ASTVarsSub> var_decls;
        ASTMetDecl (string rt, string id, vector<ASTVarsSub> vars, ASTBlock &b)
        {
            ret_type = rt;
            id_ = id;
            bl = &b;
            var_decls = vars;
        }
        ASTMetDecl (string rt, string id, ASTBlock &b)
        {
            ret_type = rt;
            id_ =id;
            bl = &b;
        }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};


class ASTVarsSub : public ASTNode
{
    public:
        string type_id;
        vector<string> vars;

        ASTVarsSub(string id, vector<string> v)
        {
            type_id = id;
            vars = v;
        }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class ASTExpr: public ASTNode
{
    public:
        ASTMethodCall * m;
        ASTBinexpr *be;
        ASTLocation *lc;
        ASTLiteral *li;
        ASTUnexpr *ue;
        int flag;
        ASTExpr (ASTLiteral &ili)
        {
            
            li = &ili;
            flag = 3;
        }
        
        ASTExpr(){}
        
        
        ASTExpr( ASTMethodCall &  im)
        {
            m = &im;
            flag = 0;
        }
        ASTExpr(ASTBinexpr &ibe)
        {
            be = &ibe;
            flag = 1;
        }
        ASTExpr(ASTLocation &ilc)
        {
            lc = &ilc;
            flag = 2;
        }
        
        ASTExpr (ASTUnexpr &iue)
        {
            ue = &iue;
            flag = 4;
        }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class ASTAssignmentStmt : public ASTStatement
{
    public:
        ASTLocation *lval;
        string op;
        ASTExpr e_;
        ASTAssignmentStmt ( ASTLocation &val, string iop, ASTExpr ine)
        {
            lval = &val;
            op = iop;
            e_ = ine;
        }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class ASTReturnStmt : public ASTStatement 
{
    public:
        ASTExpr *e_;
        int flag;
        ASTReturnStmt(){flag=1;}// No return value along with return statement
        ASTReturnStmt(ASTExpr &ine)
        {
            e_=&ine;
            flag=0;
        };
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class ASTBreakStmt : public ASTStatement
{
};

class ASTContStmt : public ASTStatement
{
};

class ASTAssign :public ASTNode
{
    public:
        string id_;
        ASTExpr e_;
        ASTAssign()
        {}
        ASTAssign (string id, ASTExpr ine)
        {
            id_ = id;
            e_ = ine;
        }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class ASTForStmt : public ASTStatement
{
    public:
        ASTAssign ass;
        ASTExpr e_;
        ASTBlock bl;
        ASTForStmt (ASTAssign as, ASTExpr e, ASTBlock b)
        {
            ass = as;
            e_ = e;
            bl = b;
        }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class ASTIFStmt : public ASTStatement
{
    public:
        ASTExpr e_;
        ASTBlock ifb;
        ASTBlock elb;
        ASTIFStmt(ASTExpr e, ASTBlock ifbl, ASTBlock elbl)
        {
            e_ = e;
            ifb = ifbl;
            elb = elbl;
        }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};


class ASTMethodCall : public ASTExpr, ASTStatement
{
    public:
        ASTNormalMethodCall *nm;
        ASTCalloutMethodCall * cm;
        int flag;
        ASTMethodCall(){}
        ASTMethodCall(ASTNormalMethodCall &inm)
        {
            nm = &inm;
            flag = 0;
        }
        ASTMethodCall(ASTCalloutMethodCall &icm)
        {
            cm = &icm;
            flag=1;
        }
	virtual llvm::Value* codeGen(CodeGenContext& context);
        
};

class ASTNormalMethodCall : public ASTMethodCall
{
    public:
        string id_;
        vector<ASTExpr> el_;
        ASTNormalMethodCall(string id, vector<ASTExpr> el) //not using pointer here remember to put *$2
        {
            id_ = id;
            el_ = el;
        }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class ASTCalloutSub : public ASTNode
{
    public:
        string strlit;
        ASTExpr e_;
        int flag;
        ASTCalloutSub (string inc)
        {
            strlit = inc;
            flag = 1;
        }
        ASTCalloutSub (ASTExpr ine)
        {
            e_ = ine;
            flag = 0;
        }
};


class ASTCalloutMethodCall : public ASTMethodCall
{
    public:
        string strlit;
        vector<ASTCalloutSub> argmt;
        ASTCalloutMethodCall(string inc, vector<ASTCalloutSub> args)
        {
            strlit = inc;
            argmt = args;
        }
};

class ASTBinexpr : public ASTExpr
{
    public:
        string op_;
        ASTExpr lhs;
        ASTExpr rhs;
        ASTBinexpr( ASTExpr llhs,string opr, ASTExpr rrhs)
        {
            op_ = opr;
            lhs = llhs;
            rhs = rrhs;
        }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class ASTUnexpr : public ASTExpr
{
    public :
        string op_;
        ASTExpr sc;
        ASTUnexpr( string op, ASTExpr ss)
        {
            op_ = op;
            sc = ss;
        }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class ASTLocation :public ASTExpr
{
    public :
        string id_;
        ASTExpr e_;
        int flag;
        void print()
        {
        }
        ASTLocation(string id, ASTExpr ine)
        {
            id_ = id;
            e_ = ine;
            flag = 0;
        }
        ASTLocation(string id)
        {
            id_ = id;
            flag = 1;
        }
        
};

class ASTLiteral : public ASTExpr
{
    public:
        string val ;
        ASTLiteral(){}
        ASTLiteral(string value)
        {
            val = value;
        }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class ASTIntLiteral : public ASTLiteral
{
    public:
        string val;
        ASTIntLiteral (string  value)
        {
            val = value;
        }
        void setvalue(string value)
        {
            val = value;
        }
};

class ASTBoolLiteral : public ASTLiteral
{
    public:
        string val;
        ASTBoolLiteral (string value)
        {
            val = value;
        }
        void setvalue(string value)
        {
            val = value;
        }
};





