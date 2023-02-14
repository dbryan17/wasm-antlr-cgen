// var myRun;

createModule().then((Module) => {
  let run_script = Module.cwrap("run_script", "null", ["string"]);
  run_script("iterate z^2 - 1 until z escapes.");
  //myRun = run_script;
  // console.log("ran");
  document.querySelector("#inBtn").addEventListener("click", () => {
    let input = document.querySelector("#scrIn").value.toLowerCase();
    run_script(input);
  });
});

// mandlebrot vvvvv

// ITERATE z^2 - 1 until z escapes.
