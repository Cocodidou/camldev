(* prodtype.ml - Print functions for generic trees with product types
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
#open \\\"format\\\";;
let rec generic_print_prodtree print_fun = function
	| \" SubstituteTree nil SendCaml \" -> ();
	| \" SubstituteTree nodeexpr SendCaml \" -> (
		print_char `(`;
		print_fun \" SubstituteTree label SendCaml \";
		print_char `[`;
		generic_print_prodforest print_fun \" SubstituteTree sons SendCaml \";
		print_char `]`;
		print_char `)`;
	)
and generic_print_prodforest print_fun = function
	| [] -> ();
	| node::rem -> (
		generic_print_prodtree print_fun node;
		generic_print_prodforest print_fun rem;
	)
;;
let print_prodtree print_fun t = 
   print_string \\\"--LemonTree--\\\";
   generic_print_prodtree print_fun t;
   print_string \\\"--EndLemonTree--\\\";
and print_prodforest print_fun t =
   print_string \\\"--LemonTree--\\\";
   generic_print_prodforest print_fun t;
   print_string \\\"--EndLemonTree--\\\";
;;
let print_int_prodtree = print_prodtree print_int
and print_int_prodforest = print_prodforest print_int
and print_string_prodtree = print_prodtree print_string
and print_string_prodforest = print_prodforest print_string
and print_char_prodtree = print_prodtree print_char
and print_char_prodforest = print_prodforest print_char
;;\n\" SetupPrinter print_int_prodtree SetupPrinter print_int_prodforest SetupPrinter print_string_prodtree SetupPrinter print_string_prodforest SetupPrinter print_char_prodtree SetupPrinter print_char_prodforest--EndLemonCamlCommand--"
;;





