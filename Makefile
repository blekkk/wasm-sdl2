public/tryWasm.wasm: src/tryWasm.mjs
src/tryWasm.mjs: src/tryWasm.cpp
	emcc --bind src/tryWasm.cpp -o src/tryWasm.mjs  \
	  --pre-js src/locateFile.js  \
	  -s ENVIRONMENT='web'  \
	  -s EXPORT_NAME='createModule'  \
	  -s USE_ES6_IMPORT_META=0  \
	  -s EXPORTED_FUNCTIONS='["_tryin"]'  \
	  -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]'  \
	  -s USE_SDL=2 \
	  -O3
	mv src/tryWasm.wasm public/tryWasm.wasm

.PHONY: clean
clean:
	rm public/tryWasm.wasm src/tryWasm.mjs