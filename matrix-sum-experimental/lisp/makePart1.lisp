#!/usr/bin/sbcl --script
(load (compile-file "part1.lisp"))
(sb-ext:save-lisp-and-die "part1" :toplevel #'main :executable t)
