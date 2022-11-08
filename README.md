# WhatsThatFigure - Pictionary Game

## Multi Threaded Server and Application

WhatsThatFigure is a Multiplayer game developed using Qt Framework.
Application uses Qt's libraries to Render Graphical User Interface,
Handle Multi Threading Communication, Networking and Object Serialization.

Server is responsible for handling Client Connections.
It creates unique Worker objects in Separate threads for each client that connects to the server. 

Worker objects collect data received from client Application. Client App may send two types of data, 
either a Message in Json format or a raw bit Stream which represents a PixelMap Object.
The Json Message is used for game logic, so it is passed to the server thread, while the 
PixelMap object is passed along to all other worker threads.

The User Application is also multi threaded and it is split into two functionalities: 
- Rendereing and displaying UI Components
- Sending and Receiving data from the Server.

The behaviour of UI and Client depends on the current player state.
In case player is currently in a Painter State, client periodically receives pixelMap data
and sends it to the server, the data is than passed to all other players so they can see the drawing.
In case player is in a Guesser State, the client simply sends players guesses for what the painter is drawing.

![screenshot](https://github.com/gaurovgiri/WhatsThatFigure/blob/main/screenshot.png?raw=true)
