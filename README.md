# WebServer
Need to figure out how to get this to run
server.c logic (launch function) contradicts everything else
HTTPServer.c in the httpServerConstructor sets the launch function, so as long as this runs we don't need the launch function as a parameter in server.c
use the launch function in HTTPServer.c as what we are trying to pass through on the main method
    -> need to fix the issues regarding this function, and other issues inside HTTPServer.c
fernando alonso the goat


okay so
server.c constructor takes the launch function as a param
HTTPServer.c constructor doesn't
we shouldn't need to change the function headers in order to make this work then :)
ngl this is cooked
the issue is in the launch funciton definition, where it's being stored, and what the singular parameter is (HTTPServer *server vs Server *server)
need to go back through the vid and figure out which param he is using for the example

started to fix the issue
"Failed to bind socket"