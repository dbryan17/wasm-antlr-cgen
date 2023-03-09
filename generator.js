// https://kapadia.github.io/emscripten/2013/09/13/emscripten-pointers-and-pointers.html

// initial setup

var canWidth = 480;
var canHeight = 270;

var mandCanvas = document.querySelector("#mandlbrotCanvas");
var mandCtx = mandCanvas.getContext("2d");
mandCtx.imageSmoothingEnabled = false;
mandCanvas.width = canWidth;
mandCanvas.height = canHeight;

// length of array - will always stay at this even if we don't fill it all the way
var arrayLength = canWidth * canHeight * 4;

// variable intializtion for stuff that will be used later on redraws
var prevStartX;
var prevStartY;
var prevWidthScale;
var prevHeightScale;

var myModule;
var myGenPixles;

createModule().then((Module) => {
  let cgen = Module.cwrap("cgen", "null", [
    "string", // script
    "number", // string pointer
  ]);

  document.querySelector("#inBtn").addEventListener("click", () => {
    let input = document.querySelector("#scrIn").value.toLowerCase();

    // Trying to do this new memeory way instead of the more complex buffers

    let strPtr = Module.allocateUTF8(1000); // allocate memory available to the emscripten runtime and create a pointer
    // let strPtr = Module._malloc(1000 * Uint8Array.BYTES_PER_ELEMENT);

    cgen(input, strPtr);

    code = Module.UTF8ToString(strPtr); // read from the allocated memory to the javascript string

    console.log(code.trim());

    Module._free(1000); // release the allocated memory

    // free the memory
  });
});
