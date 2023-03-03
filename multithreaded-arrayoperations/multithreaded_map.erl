% Does work, but it creates a new thread for each element in the list.

-module(multithreaded_map).
-compile(export_all).

map(NumberOfThreads,Function,List) ->
        ParentPid = self(),
        lists:flatten([receive
                               {Pid,Result} ->
                                       Result
                       end
                       || Pid <- [spawn(fun() ->
                                                        io:fwrite("#~w: ~w~n",[self(),L]),
                                                        ParentPid ! {self(),lists:map(Function,L)}
                                        end)
                                  || L <- [X || X <- group2(NumberOfThreads,List)]]]).

group(N,L) ->
        group(N,L,[]).
group(_,[],Acc) ->
        Acc;
group(_,[H|[]],Acc) ->
        Acc ++ [[H]];
group(N,L,Acc) ->
        {H,T} = lists:split(N,L),
        group(N,T,Acc ++ [H]).

group2(N,L) ->
        Length = length(L),
        Split = Length div N,
        Remainder = Length rem N,
        InnerFun = fun Rec(I,_,Output) when I > N ->
                                   Output;
                       Rec(I,Input,Output) when I =< Remainder ->
                                   {Head,Tail} = lists:split(Split + 1, Input),
                                   Rec(I + 1,Tail,Output ++ [Head]);
                       Rec(I,Input,Output) ->
                                   {Head,Tail} = lists:split(Split, Input),
                                   Rec(I + 1,Tail,Output ++ [Head])
                   end,
        InnerFun(1,L,[]).
