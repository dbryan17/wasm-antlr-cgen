// https://tomassetti.me/listeners-and-visitors/
// https://tomassetti.me/getting-started-antlr-cpp/

// not including the runtime - is in demo, difference with the ex from gtihub, probablty needed
// for wasm - understand this more

#include <iostream>
#include <stdio.h>
#include "FractalParser.h"
#include "FractalLexer.h"
#include "FractalBaseVisitor.h" 
#include "FractalVisitor.h"
#include "antlr4-runtime.h"
#include "MyVisitor.h"
#include <complex.h>
#include <emscripten/emscripten.h>
// #include "libantlr4-runtime.a"



// this if thing in ex makes it so it runs the emscripten way if emscripten is defined?,
// then the else block runs the lexer+parser in the more normal way that is shown in the examples online 
#ifdef __EMSCRIPTEN__

// #include <emscripten.h>

using namespace std;
using namespace antlr4;

int main(int argc, const char* argv[]) { return 0; }
int getIdx(int x, int y, int width, int color);


int getIdx(int x, int y, int width, int color)
{
  int red = y * (width * 4) + x * 4;
  return red + color;
}





// function for cwrap here 
extern "C" {



  

  // EMSCRIPTEN_KEEPALIVE void run_script(const char *stream, int type, double fixedVarRe, double fixedVarIm, double critRe, double critIm, 
  // double startX, double startY, double newCanWidth, double newCanHeight, int width, int height, double widthScale, double heightScale,  uint8_t *ptr) {

    EMSCRIPTEN_KEEPALIVE void cgen(const char *stream, uint8_t *ptr) {
      std::cout << "HEREERERE\n";

    // below is how they are doing it in online example because that is the more up to date
    // version of antlr, slightly different than how they are doing it in wasm ex, but the 
    // boilerplate that is being generated by antlr is the one from the most recent version
    // might be weird though because online they are inlcuding the runtime that we don't have
    // gonna have to try both I'm assuming

    // std:cout << "in functin\n";


    

    // turn the input into antlr format 
    antlr4::ANTLRInputStream input(stream);
    // antlr4::ANTLRInputStream input_stream(input);

    // creaate lexer, get tokens, parse
    FractalLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    FractalParser parser(&tokens);

    // call script method - this scriptContext contains method to access the text caputred by the rule
    FractalParser::ScriptContext* tree = parser.script();

    // variable to set critical point to - FOR DYN SPACE ONLY - have to figure this out later
    std::string crit = "z"; // can have this set by the user 
    // variable to set screen point to 
    std::string screen = "c"; // can have this set by the user
    std::complex<double> crit_point(0.,0.); 

        myVisitor visitor(16, 4., 0.1, crit, screen, crit_point);


    int modcount = 0;
    std::cout << "HEREERERE\n";

    std::string code = visitor.cgen(tree);

    ///// see if this works???? /////
    //ptr[0] = reinterpret_cast<const uint8_t *>(code.c_str());
    std::cout << "HEREERERE\n";
    for(int i =0; i < code.length(); i++) {
      ptr[i] = code.at(i);
      std::cout << ptr[i]<< "\n";
    }
    ptr[code.length()] = '\0';

    std::cout << "all done\n";



    

  //   for (int x = 0; x < floor(newCanWidth); ++x)
  // {
  //   for (int y = 0; y < floor(newCanHeight); ++y)
  //   {



  //     double new_x = (((widthScale * x) + startX) - width / 2.) / (height / 2.) - .55;
  //     double new_y = -(((heightScale * y) + startY) - height / 2.) / (height / 2.);

  //     int iterations;

  //     if(type == 0) {
  //       // eval point takes a point, and evals the script with it - currently set up for paramter
  //       // so point gets set to the screen point (c)
  //       iterations = visitor.evalPoint(std::complex<double>(new_x, new_y), tree);// mandlebrot(new_x, new_y);
  //       // std:cout << "evaled a point: " << iterations << "\n"; 
  //     } 
  //     // else if(type == 1) {
  //     //   iterations = julia(new_x, new_y, cRe, cIm);
  //     // }
  //     // std:coutt << "adding point at " << x << y << width << iterations*4 << "\n";
  //     // std:coutt << "x: " << x << "y: " << y << "\n";
  //     ptr[getIdx(x, y, width, 0)] = iterations * 16;
  //     // std:cout << "first get idx\n";
  //     ptr[getIdx(x, y, width, 3)] = 255;
  //     // std:cout << "second get idx\n";

  //     //// imageSmoothingEnabled to false **** this might save time, and also make it
  //     // look less weird when you zoon in
  //   }
  // }





    // do loop here 
    //int iterations = visitor.evalPoint(std::complex<double>(1.5,1.5), tree);
    

    // visitor.visitScript(tree);
    // fractal.//
    //printf("testing %d\n", iterations);




  }
}

  #else

  int main(int, const char **) { return 0; }

  


#endif