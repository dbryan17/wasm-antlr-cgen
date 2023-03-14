// https://kapadia.github.io/emscripten/2013/09/13/emscripten-pointers-and-pointers.html

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
    document.querySelector("#output").innerText = code.trim();

    Module._free(1000); // release the allocated memory

    // free the memory
  });
});
