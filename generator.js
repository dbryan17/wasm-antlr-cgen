// https://kapadia.github.io/emscripten/2013/09/13/emscripten-pointers-and-pointers.html

var myModule;
var myGenPixles;

createModule().then((Module) => {
  let cgen = Module.cwrap("cgen", "number", [
    "string", // script
  ]);
  let getCgen = Module.cwrap("getCgen", "number", [
    "number", // string pointer
  ]);

  document.querySelector("#inBtn").addEventListener("click", () => {
    let input = document.querySelector("#scrIn").value.toLowerCase();
    console.log("about to get length");

    length = cgen(input);
    console.log(length);

    // Trying to do this new memeory way instead of the more complex buffers

    let strPtr = Module.allocateUTF8(length); // allocate memory available to the emscripten runtime and create a pointer
    // let strPtr = Module._malloc(1000 * Uint8Array.BYTES_PER_ELEMENT);

    type = getCgen(strPtr);

    code = Module.UTF8ToString(strPtr); // read from the allocated memory to the javascript string

    console.log(type);
    console.log(code.trim());
    document.querySelector("#output").innerText = code.trim();

    Module._free(strPtr); // release the allocated memory

    // free the memory
  });
});
