% Does work, but it creates a new thread for each element in the list.

-module(multithreaded_map).
-compile(export_all).

map(NumberOfThreads,Function,List) ->
        ParentPid = self(),
        [receive
                 {Pid,Result} ->
                         Result
         end
         || Pid <- [spawn(fun() ->
                                          ParentPid ! {self(),Function(X)}
                          end)
                          || X <- List]].
