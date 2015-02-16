#include<bits/stdc++.h>
#include "node.cpp"
using namespace std;


void ptab(int il)
{
    for(int i =0;i<il;i++)
        printf("\t");
}

void field_decl_print(ASTFieldDecl,int);
void met_decl_print(ASTMetDecl, int);
void program_print(ASTProgram *p, int level)
{
    ptab(level);
    cout<<"CLASS PROGRAM\n"; 
    
    int i;
    cout<<"|--- Field Declarations\n";
    for(i=0;i<p->fdl_.size();i++)
    {
        field_decl_print(p->fdl_[i],level+1);
        cout<<endl;
    }
    cout<<"|--- Method Declarations\n";
    for(i=0;i<p->mtl_.size();i++)
    {
        met_decl_print(p->mtl_[i],level+1);
        cout<<endl;
    }
}


void field_decl_print(ASTFieldDecl f,int level)
{
    ptab(level);
    cout<<f.type_<<' ';
    int i;
    for (i=0;i<f.var.size();i++)
    {
        cout<<f.var[i].id_<<" ";
        if(!f.var[i].flag)
            cout<<"["<<f.var[i].lit_->val<<"]"<<" ";
    }
}

void block_print(ASTBlock*,int);
void met_decl_print(ASTMetDecl md, int level)
{
    ptab(level);
    cout<<"Method\n";
    ptab(level);
    cout<<md.ret_type<<" ";
    cout<<md.id_<<"\n ";
    ptab(level);
    cout<<"Parameters\n";
    ptab(level);
    for(int i=0;i<md.var_decls.size();i++)
        cout<<md.var_decls[i].type_id<<" "<<md.var_decls[i].vars[0]<<" ";
    cout<<"\n";
    ptab(level);
    cout<<"Block\n";
    ptab(level);
    block_print(md.bl,level+1);
}


void field_decl_block_print(ASTVarsSub,int);
void expr_print(ASTExpr *);
void met_call_print(ASTMethodCall *);
void ifstmt_print(ASTIFStmt *,int);
void assstmt_print(ASTAssignmentStmt *, int);
void forstmt_print(ASTForStmt *,int);
void block_print(ASTBlock *ibl,int level)
{
    cout<<"{\n";
    if(ibl->v_sub.size())
    {    ptab(level);
         cout<<"Local Variable declarations\n";
    }
    //ptab(level);
    int i; 
    for(i=0;i<ibl->v_sub.size();i++)
    {
        ptab(level);
        cout<<"|--- ";
        field_decl_block_print(ibl->v_sub[i],level-1);
        cout<<"\n";
    }
    if(ibl->stmts.size())
    {    ptab(level);
    cout<<"Statements\n";}
    for(i=0;i<ibl->stmts.size();i++)
    {
        ptab(level);
        cout<<"|--- ";
        if(ibl->stmts[i].flag == 2)
            cout<<"Continue ";
        else
        if(ibl->stmts[i].flag == 6)
            cout<<"Break ";    
        else
        if(ibl->stmts[i].flag == 7)
            met_call_print(ibl->stmts[i].mc);
        else
        if( ibl->stmts[i].flag == 0)
            assstmt_print(ibl->stmts[i].ass,level+1);
        else
        if(ibl->stmts[i].flag == 5)
            ifstmt_print(ibl->stmts[i].ifel,level+1);
        else
        if(ibl->stmts[i].flag == 4)
            forstmt_print(ibl->stmts[i].fr,level+1);
        else
        if(ibl->stmts[i].flag == 3)
            block_print(ibl->stmts[i].blk,level+1);
        else
        {
            cout<<"Return ";
            if(!ibl->stmts[i].flag)
                expr_print(ibl->stmts[i].rt->e_);
        }
        cout<<"\n";
    }


    ptab(level-1);
    cout<<"}";
}


void callout_sub_print(ASTCalloutSub *cb)
{
	if(cb->flag == 1)
		cout<<cb->strlit;

	if(cb->flag == 0)
		expr_print(&(cb->e_));
}


void met_call_print(ASTMethodCall* m)
{
	if(m->flag == 0)
	{
		cout<<m->nm->id_<<" ";
		cout<<"( ";
		int i;

		for(i=0;i<m->nm->el_.size();i++)
		{
			expr_print(&m->nm->el_[i]);
			cout<<" , ";
		}
		cout<<")";
	}


	if(m->flag == 1)
	{
		cout<<"CALLOUT "<<m->cm->strlit<<" ,";
		int i;

		for(i=0;i<m->cm->argmt.size();i++)
		{
			callout_sub_print(&m->cm->argmt[i]);
			cout<<" ,";
		}
	}
}


void assign_print(ASTAssign *a)
{
	cout<<a->id_<<"= ";
	expr_print(&a->e_);
}


void forstmt_print(ASTForStmt *f,int level)
{

	
	cout<<"FOR ";
	assign_print(&f->ass);
	cout<<" ,  ";
	expr_print(&f->e_);
	cout<<endl;
        ptab(level);
    	cout<<"Block\n";
	ptab(level);
        block_print(&f->bl,level+1);
	cout<<endl;
	
}




void ifstmt_print(ASTIFStmt *i,int level)
{

	cout<<"IF ";
	expr_print(&i->e_);
	cout<<"\n";
        ptab(level);
    	cout<<"Block\n";
	ptab(level);
        block_print(&i->ifb,level+1);
	cout<<"\n";
	ptab(level);
	cout<<"ELSE ";
    	cout<<"Block\n";
	ptab(level);
        block_print(&i->elb,level+1);
	cout<<"\n";
}

void location_print(ASTLocation *l)
{
	if(l->flag == 0)
	{
		cout<<l->id_<<" [  ";
		expr_print(&l->e_);
		cout<<" ]";
	}

	else
	{
		cout<<l->id_;

	}
}


void assstmt_print(ASTAssignmentStmt *a,int level)
{
	location_print(a->lval);
	cout<<" "<<a->op<<" ";
	expr_print(&a->e_);
}


void binex_print(ASTBinexpr *b)
{
	expr_print(&b->lhs);

	cout<<" "<<b->op_<<" ";

	expr_print(&b->rhs);
}

void unex_print(ASTUnexpr *u)
{
	cout<<u->op_<<" ";

	expr_print(&u->sc);
}

void literal_print(ASTLiteral *l)
{
	cout<<l->val;
}

void expr_print(ASTExpr *e)
{
	
	cout<<"( ";
	if(e->flag == 0)
	{
		met_call_print(e->m);
	}

	else
	if(e->flag == 1)
	{	
		binex_print(e->be);
	}

	else
	if(e->flag == 2)
	{
		location_print(e->lc);
	}

	else
	if(e->flag == 3)
	{
		literal_print(e->li);
	}

	else
	if(e->flag == 4)
	{
		unex_print(e->ue);
	}

	cout<<" )";
}




void field_decl_block_print(ASTVarsSub f,int level)
{
//    ptab(level);
    cout<<f.type_id<<' ';
    int i;
    for (i=0;i<f.vars.size();i++)
    {
        cout<<f.vars[i]<<" ";
        //if(!f.vars[i].flag)
        //    cout<<"["<<f.vars[i].lit_->val<<"]"<<" ";
    }
}




