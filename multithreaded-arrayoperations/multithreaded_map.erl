-module(multithreaded_map).
%-compile(export_all).
-export([map/3]).

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
                                  || L <- [X || X <- group(NumberOfThreads,List)]]]).

group(N,L) ->
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

group2(N,L) ->
        group(N,L,[]).
group2(_,[],Acc) ->
        Acc;
group2(_,[H|[]],Acc) ->
        Acc ++ [[H]];
group2(N,L,Acc) ->
        {H,T} = lists:split(N,L),
        group(N,T,Acc ++ [H]).
