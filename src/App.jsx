import ReactDOM from "react-dom";
import React, { useState, useEffect } from "react";
import createModule from "./tryWasm.mjs";
import { useRef } from 'react';

function App() {
    const [tryWasm, settryWasm] = useState();
    const canvasRef = useRef(null);

    useEffect(() => {
        createModule().then((Module) => {
            let canvas = canvasRef.current;
            Module["canvas"] = (() => {
                // As a default initial behavior, pop up an alert when webgl context is lost. To make your
                // application robust, you may want to override this behavior before shipping!
                // See http://www.khronos.org/registry/webgl/specs/latest/1.0/#5.15.2
                canvas.addEventListener("webglcontextlost", (e) => { alert('WebGL context lost. You will need to reload the page.'); e.preventDefault(); }, false);

                return canvas;
            })()
            settryWasm(() => Module.cwrap("tryin"));

        });
    }, []
    );

    useEffect(() => {
        if (tryWasm) {
            tryWasm();
        }
    }, [tryWasm])

    return (
        <div className="App">
            <canvas ref={canvasRef} className="emscripten" id="canvas"></canvas>
        </div>
    );
}

ReactDOM.render(<App />, document.getElementById("app"));
