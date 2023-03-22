-module(quicksort).
-export([sort/1,concurrentSort/1]).

sort([]) ->
        [];
sort([Pivot|Tail]) ->
        sort([X || X <- Tail, X < Pivot])
        ++
        [Pivot]
        ++
        sort([X || X <- Tail, X >= Pivot]).

concurrentSort([]) ->
        [];
concurrentSort([Pivot|Tail]) ->
        ParentPid = self(),
        LesserThread = spawn(fun() -> ParentPid ! {self(),sort([X || X <- Tail, X < Pivot])} end),
        HigherThread = spawn(fun() -> ParentPid ! {self(),sort([X || X <- Tail, X >= Pivot])} end),
        Loop = fun Rec(0,Acc) ->
                               Acc;
                   Rec(N,Acc) ->
                               receive
                                       {LesserThread,Result} ->
                                               Rec(N - 1, Result ++ Acc);
                                       {HigherThread,Result} ->
                                               Rec(N - 1, Acc ++ Result)
                               end
               end,
        Loop(2,[]).
