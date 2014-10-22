(* sumtype.ml - Print functions for generic trees with sum types
 This file is part of LemonCaml - Copyright (C) 2012-2014 Corentin FERRY
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>. *)

print_string "--LemonCamlCommand--SendCaml \"
#open \"format\";;
let rec generic_print_sumtree print_fun t = 
   print_char `(`;
   print_fun t.\" SubstituteTree label SendCaml \";
   print_char `[`;
   generic_print_sumforest print_fun t.\" SubstituteTree sons SendCaml \";
   print_char `]`;
   print_char `)`;
and generic_print_sumforest print_fun = function
   | [] -> ();
   | node::rem -> ( 
      generic_print_sumtree print_fun node;
      generic_print_sumforest print_fun rem;
   )
;;
let print_sumtree print_fun t = 
   print_string \"--LemonTree--\";
   generic_print_sumtree print_fun t;
   print_string \"--EndLemonTree--\";
and print_sumforest print_fun t =
   print_string \"--LemonTree--\";
   generic_print_sumforest print_fun t;
   print_string \"--EndLemonTree--\";
;;
let print_int_sumtree = print_sumtree print_int
and print_int_sumforest = print_sumforest print_int
and print_string_sumtree = print_sumtree print_string
and print_string_sumforest = print_sumforest print_string
and print_char_sumtree = print_sumtree print_char
and print_char_sumforest = print_sumforest print_char
;;\n\" SetupPrinter print_int_sumtree SetupPrinter print_int_sumforest SetupPrinter print_string_sumtree SetupPrinter print_string_sumforest SetupPrinter print_char_sumtree SetupPrinter print_char_sumforest--EndLemonCamlCommand--"
;;

