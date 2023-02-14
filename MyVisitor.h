  
#include "FractalBaseVisitor.h"
#include <stdio.h>
#include <iostream>
#include "antlr4-runtime.h"
#include <complex.h>
#include <unordered_map>



  // If c does not appear, then FractalStream will draw a picture in the dynamical plane. 
  // Since the script used vanishes instead of escapes, we colored the interior instead of the exterior. 



  class  myVisitor : public FractalBaseVisitor {


    // hash table of variable to value 
    std::unordered_map<std::string,std::complex<double>> inVars = {};

    // std::string currVar;

    // std::complex<double> result;
    

    // how many iterations to go to get to the bailout value
    int iters;
    // when to call it infinity - 2 for mandlebrot set 
    int maxRadius;

    std::string vari;

public: 
  myVisitor(int iters, int maxRadius, std::string& var) : iters(iters), maxRadius(maxRadius), vari(var) {}

  // function takes a point in complex plane, and evaluates it with the script
  int evalPoint(std::complex<double> point, FractalParser::ScriptContext* tree) {

    // set variable we are changing to the point
    inVars[vari] = point;
    // get result
    int res = visitScript(tree);
    // clear instance variables
    inVars.clear();
    return res;
  }

  
  virtual int visitScript(FractalParser::ScriptContext *ctx) override {
    visitChildren(ctx);
    std::cout << "done running script\n";
    // not sure how to find iterations to return, will also eventaully be a color - maybe for now get an variable
    return 6;

  }


  //////////////////////////////////////
  /////////////// commands /////////////
  //////////////////////////////////////

  // return result of expression 
  virtual std::complex<double> visitSET_TO_COM(FractalParser::SET_TO_COMContext *ctx) override {
    std::cout << "in settocom\n";
    visitChildren(ctx);
    std::cout << "visited children of settocom\n";
    return std::complex<double>(4.,3.);
  }

  // return result of the expression 
  // this is the same as set but allows it to be "edited in params tab in setting window"
  virtual std::complex<double> visitDEFAULT_TO_COM(FractalParser::DEFAULT_TO_COMContext *ctx) override {
    return visitChildren(ctx);
  }

  // only evalute if drawing paramter space - figure out return later 
  virtual antlrcpp::Any visitPAR_COM(FractalParser::PAR_COMContext *ctx) override {
    return visitChildren(ctx);
  }
  // only evalte if drawing dynamic space - figure out return later
  virtual antlrcpp::Any visitDYN_COM(FractalParser::DYN_COMContext *ctx) override {
    return visitChildren(ctx);
  }

  // don't think I'll need to override any of the other ones 



  ////////////// end ///////////////////
  /////////////// commands /////////////
  //////////////////////////////////////






  //////////////////////////////////////
  /// atom, variable, constant /////////
  ///////////// numbers ////////////////


  /*
  Either a variable or a constant
  */
  virtual std::complex<double> visitAtom(FractalParser::AtomContext *ctx) override {

  return visitChildren(ctx);
  }


  /*
  This is simply a vairable, will have a value which is a complex number
  Will only want to update variables at speicific times, like looping
  z = z^2 + z + 45, we only want to update the value of z when one iteration
  ends
  Returns: value of variable at last assignment
  */
   virtual std::complex<double> visitVariable(FractalParser::VariableContext *ctx) override {
    /* 
    - add variable to the instance variable map if not there - and assign it a value based on the script/ critical point 
    - really should always have a value. 
    */

    std::complex<double> val;
    std::string text = ctx->getText();

    // check if variable is in hashmap 

    // variable in map
    if (inVars.count(text) != 0) {
      // currVar = ctx->getText();
      // get value
    // not in map 
    } else {
      // TODO get this from user
      std::complex<double> critPoint(1.2,3.6);
      inVars.insert(make_pair(text, critPoint));
      // set to curr ---- for now, with the par and stuff will want to do different stuff 
      // currVar = ctx->getText();
    }

    val = inVars[text];

    return val;
  }

  virtual std::complex<double> visitConstant(FractalParser::ConstantContext *ctx) override {
    // children will garunteed return a complex number, so hopefully this is fine 
    return visitChildren(ctx);
  }


  // for pow expression - I get the value a different way, so this is just for 
  // complex numbers
  virtual std::complex<double> visitN(FractalParser::NContext *ctx) override {
    /// int n = stoi(ctx->n()->getText());
    return std::complex<double>(0.,0.);
  }

  // real part of complex number
  virtual std::complex<double> visitCpx_number_re(FractalParser::Cpx_number_reContext *ctx) override {
    // (text, 0)
    return std::complex<double>(1.,0.);
  }

  virtual std::complex<double> visitCpx_number_im(FractalParser::Cpx_number_imContext *ctx) override {
    // check if it is just i, if it is, then (0,1) otherwise (0,text-i)
    return std::complex<double>(0.,1.);
  }


  ////////////// end ///////////////////
  /// atom, variable, constant /////////
  //////////// numbers /////////////////

  //////////////////////////////////////
  //////////  EXPRESSIONS //////////////
  //////////////////////////////////////

  /*
  These are all the expressions 
  They return the result of the expression 
  */


 /*
 This is either a constant or a variable 
 Return the value of that constant or variable
 */
  virtual std::complex<double> visitSIGNED_ATOM_EXP(FractalParser::SIGNED_ATOM_EXPContext *ctx) override {\

    // has a signed applied to it here, get value of atom, then apply the sign to it if there is one

    // visitChildren(ctx);

    return std::complex<double>(4.0,3.1);
  }

  // might have to do some special order of operations stuff here, but don't really think so
  virtual std::complex<double> visitPAREN_EXP(FractalParser::PAREN_EXPContext *ctx) override {
    return visitChildren(ctx);
  }

  // this is only allowed for z^n when n is a positive integer which is error checked in parser
  virtual std::complex<double> visitPOW_EXP(FractalParser::POW_EXPContext *ctx) override {

    /* 
    - recursively get the variable/(expression to be assigned a variable) we are powing 
    - put it to the power of n and assign
    */

   std::cout << ctx->getText() << "      --- pow exp\n";

    // get n - this is the only use of n, so don't even need an override for it
    int n = stoi(ctx->n()->getText());

    // visit all children (should eval expressions and for now get currVar right) - really only
    // need to visit expression on right
    visitChildren(ctx);

    // FOR NOW: need to look up what val currVal is at and put it to the power of n 
    // std::complex<double> inVarVal(inVars[currVar]);
    // std::cout << inVarVal << "\n"; // 
    // std::complex<double> result = pow(inVarVal, n);
    // inVars[currVar] = pow(inVarVal, n);
    // std::cout << inVars[currVar] << "\n"; //

    // guesss ctx works as a return, don't really need to return anything
    // return result;
    return std::complex<double>(3., 2.);
  }

  virtual std::complex<double> visitPLUS_EXP(FractalParser::PLUS_EXPContext *ctx) override {
    /*
    - FOR NOW: assuming z is being set to result of this, could return result but don't need to
    - recursively visit expression on left of +
    - recursively visit expression on right of + 
    - add them and assign
    - just have expressions return result
    */

    // get left side exp
    // antlrcpp::Any left = ctx->left(); //  - not working

    std::cout << "plus\n";

    // this works

   

    std::complex<double> left = visit(ctx->expression(0));
    std::complex<double> right = visit(ctx->expression(1));

    std::cout << "plus done\n";



    return left+right;
    

 


    // antlrcpp::Any l = visitChildren(ctx->expression(0));
    // antlrcpp::Any r = ctx->expression(1);

    // std::complex<double> toR = l+r;

    //return toR;
    // return ctx;

  }

    virtual std::complex<double> visitTIMES_EXP(FractalParser::TIMES_EXPContext *ctx) override {
    return visitChildren(ctx);
  }

    virtual std::complex<double> visitDIVIDE_EXP(FractalParser::DIVIDE_EXPContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::complex<double> visitMINUS_EXP(FractalParser::MINUS_EXPContext *ctx) override {
    return visitChildren(ctx);
  }

    virtual std::complex<double> visitREDUCE_MOD_EXP(FractalParser::REDUCE_MOD_EXPContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::complex<double> visitRE_FCN_EXP(FractalParser::RE_FCN_EXPContext *ctx) override {
    return visitChildren(ctx);
  }

    virtual std::complex<double> visitCPX_FCN_EXP(FractalParser::CPX_FCN_EXPContext *ctx) override {
    return visitChildren(ctx);
  }


  /////////////// end //////////////////
  //////////  EXPRESSIONS //////////////
  //////////////////////////////////////



  //////////////////////////////////////
  /////////// CONDITIONS ///////////////
  //////////////////////////////////////

    virtual bool visitSTOPS_COND(FractalParser::STOPS_CONDContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual bool visitCOMP_COND(FractalParser::COMP_CONDContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual bool visitVANISHES_COND(FractalParser::VANISHES_CONDContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual bool visitESCAPES_COND(FractalParser::ESCAPES_CONDContext *ctx) override {
    visitChildren(ctx);
    return true;
  }

  virtual bool visitCOMB_COND(FractalParser::COMB_CONDContext *ctx) override {
    return visitChildren(ctx);
  }




  //   // will return a boolean
  //   virtual antlrcpp::Any visitCondition(FractalParser::ConditionContext *ctx) override {
  //     std::cout << "condition says: ";
  //     std::cout << ctx->getText() <<"\n";

  //     std::string t = ctx->toStringTree();
  //     std::cout << "string tree: " << t << "\n";



  //     // test all the methods in the java thing:

    
  //     // antlrcpp::Any be = ctx->getChild(0); - desont work 
  //     int be = ctx->depth(); // works 
  //     std::cout << "depth: " << be << "\n";

  //     // int ce = ctx->getChildCount(); - doesnt owkr
  //     // antlrcpp::Any ce = ctx->getParent(); - ^^
  //     // antlrcpp::Any ce = ctx->getPayload();- ^^
  //     // antlrcpp::Any ce = ctx->getRuleContext(); - ^^

  //     antlrcpp::Any parent = ctx->parent; // works!

  //     // antlrcpp:Any chi = ctx->child(1);

      
  //   return visitChildren(ctx);
  // }

  };



  ////////////// end ///////////////////
  /////////// CONDITIONS ///////////////
  //////////////////////////////////////




  //////////////////////////////////////
  //////////////  LOOPS ////////////////
  //////////////////////////////////////

  //   virtual antlrcpp::Any visitLoopDo(FractalParser::LoopDoContext *ctx) override {

  //     antlrcpp::Any var = ctx->variable();
  //     std::string varText = ctx->variable()->getText();
  //     antlrcpp::Any exp = ctx->expression();
  //     antlrcpp::Any condit = ctx->condition();

  //     // put variable in hashamp (with user defined crit point as starting value)
  //     std::complex<double> varVal = visitVariable(var);

  //     int counter = 0; 

  //     do {
  //       // eval expression 
  //       std::complex<double> eval = visitExpression(exp);
  //       // update 
  //       inVars[varText] = eval;
  //       // increment
  //       counter++;
  //     }
  //     while (!visitCondition(condit));







  //   // return visitChildren(ctx);
  // }





  //     // ITERATE expression 'until' condition
  //   virtual antlrcpp::Any visitLoopIterateEmpty(FractalParser::LoopIterateEmptyContext *ctx) override {
  //     printf("visited loop iterate empty node\n");



  //     // std::string txt = ctx->getText();

  //     // expression should be second child

  //     // std::cout << "loop node says: ";

  //     // std::cout << txt << "\n";

  //     // for this type of loop, variable is taken to be z, so z is what we are resetting - there need to be a z
  //     // some way to get the chldren or get the parse tree????? 

  //     // one free critical point -- 

  //     // std::complex<double> z(0., 0.);



     

  //     // antlrcpp::Any exp = ctx->getChild(1);
  //     // int exp = ctx->getChildCount();
  //     antlrcpp::Any exp = ctx->expression();

  //     antlrcpp::Any condit = ctx->condition();


  //     do {
  //       // eval expression
  //       // -- maybe throw in the result as a parameter
  //       std::complex<double> result = visitExpression(exp);
  //       // update changing varaible


  //     } while(true);
    

  //     antlrcpp::Any c = visitCondition(condit);

  //     // iterate z




  //     // for (int i = 1; i < iters; i++) {

  //     //     // std::complex<double> z2 = z*z + c;
  //     //     if(abs(z2) > 4) {
  //     //       return i;
  //     //     }
  //     //     reset
  //     // }






  //     // std::cout << "here\n";



  //     // antlrcpp::Any eval = visitExpression(exp);

  //     //return visitChildren(ctx);

  //     // visit

  //     return ctx;

    
  //     //return visitChildren(ctx);
      
  // }

  //////////////// end /////////////////
  //////////////  LOOPS ////////////////
  //////////////////////////////////////















  //   virtual antlrcpp::Any visitExpression(FractalParser::ExpressionContext *ctx) override {

  //     std::string txt = ctx->getText();

  //     // expression should be second child

  //     std::cout << "expression says: ";

  //     std::cout << txt << "\n";


  //   return visitChildren(ctx);
  // }









/*
-list out data types

https://stackoverflow.com/questions/71348386/emscripten-c-to-wasm-with-classes-error-undefined-symbol
*/

// https://www.antlr.org/api/Java/org/antlr/v4/runtime/RuleContext.html