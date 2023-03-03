-module(testthreadtaskdivider).
-compile(export_all).

group(N,L) ->
        group(N,L,[]).
group(_,[],Acc) ->
        Acc;
group(_,[H|[]],Acc) ->
        Acc ++ [[H]];
group(N,L,Acc) ->
        {H,T} = lists:split(N,L),
        group(N,T,Acc ++ [H]).
