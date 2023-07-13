# fractalTileMessage

Attached is code for “fractal_tiled”.
- It uses std::async and instances of “FractalMessage” to render tiles of fractals to an output Bitmap using
the method “fractal”. 
- Contains the class “FractalMessage” which encapsulates all the state needed to render a
“fractal tile” in a “Message”.
- In Main.cpp, it has a nested for-loop which creates 16 instances of FractaLMessage, one each for a
different tile, passing these messages into std::async to do the actual computation and rendering of the
fractal. std::async then renders each tile asynchronously.

- ThreadSafeQueue of FractalMessages and then a number of threads which remove the FractalMessages from the queue using “pop”, then
calling fractal() with the contents of the message.
- imported ThreadSafeQueue.h into fractal_tiled.

