#!/usr/bin/sbcl --script
(load (compile-file "part2.lisp"))
(sb-ext:save-lisp-and-die "part2" :toplevel #'main :executable t)
