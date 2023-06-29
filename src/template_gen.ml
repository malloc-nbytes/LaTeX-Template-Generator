type template_creator =
  { mutable name : string option
  ; mutable dir : string option
  ; mutable compiler : string option
  ; mutable template_file : string option
  }

let template_files =
  [ "../templates/research.template"
  ; "../templates/notes.template"
  ; "../templates/homework.template"
  ; "../templates/article.template"
  ]

let usage () : unit =
  let _ = print_endline "(USAGE) ./template_gen template=<template> compiler=pdflatex filepath=~/dev/tmp/main.tex" in
  failwith "Usage"

let err (msg) : unit =
  let _ = Printf.printf "(ERR) %s\n" msg in
  usage ()

let help () : unit =
  let _ = print_endline "template=<template>" in
  let _ = print_endline "  article" in
  let _ = print_endline "    - Creates a file with the `article` template" in
  let _ = print_endline "  notes" in
  let _ = print_endline "    - Creates a file with the `notes` template" in
  let _ = print_endline "  homework" in
  let _ = print_endline "    - Creates a file with the `homework` template" in
  let _ = print_endline "  research" in
  let _ = print_endline "    - Creates a file with the `research` template" in
  let _ = print_endline "filepath=<path/to/create/file.tex>" in
  let _ = print_endline "  - Give a path that is already created" in
  let _ = print_endline "  - It will create the `file.tex` for you" in
  let _ = print_endline "  - The filename must end with `tex`" in
  let _ = print_endline "compiler=<compiler>" in
  let _ = print_endline "  - Specify a compiler to use in the Makefile" in
  usage ()

let file_to_string (filepath : string) : string =
  let channel = open_in_bin filepath in
  let data = really_input_string
               channel
               (in_channel_length channel) in
  let _ = close_in channel in
  data

let write_to_file (filepath : string) (content : string) (filename : string) : unit =
  let _ = Printf.printf "Writing to file %s\n" filename in
  let out_channel = open_out filepath in
  output_string out_channel content;
  close_out out_channel

let get_tc_template (arg : string) : string =
  let research = 0 in
  let notes = 1 in
  let homework = 2 in
  let article = 3 in
  match arg with
  | "research" ->
     let _ = Printf.printf "Template set to %s\n" (List.nth template_files research) in
     List.nth template_files research
  | "notes" -> 
     let _ = Printf.printf "Template set to %s\n" (List.nth template_files notes) in
     List.nth template_files notes
  | "homework" -> 
     let _ = Printf.printf "Template set to %s\n" (List.nth template_files homework) in
     List.nth template_files homework
  | "article" -> 
     let _ = Printf.printf "Template set to %s\n" (List.nth template_files article) in
     List.nth template_files article
  | _ -> let _ = err "Invalid template" in ""

let get_tc_dir (filepath : string) : string =
  let dir = String.split_on_char '/' filepath
            |> List.rev
            |> List.tl
            |> List.rev
            |> String.concat "/" in
  let _ = Printf.printf "Directory set to %s\n" dir in
  dir

let get_tc_name (filepath : string) : string =
  let name = String.split_on_char '/' filepath
             |> List.rev
             |> List.hd in
  match List.hd (List.rev (String.split_on_char '.' name)) with
  | "tex" ->
     let _ = Printf.printf "Filename set to %s\n" name in
     name
  | _ -> let _ = err "Filename must end with .tex" in ""

let rec process_input (argv : string array) (argc : int) (i : int) (tc : template_creator) : template_creator =
  match i = argc with
  | true -> tc
  | false ->
     (match String.split_on_char '=' argv.(i) with
      | ["compiler"; arg] ->
         let _ = tc.compiler <- Some arg in
         process_input argv argc (i + 1) tc
      | ["filepath"; arg] ->
         let _ = tc.dir <- Some (get_tc_dir arg) in
         let _ = tc.name <- Some (get_tc_name arg) in
         process_input argv argc (i + 1) tc
      | ["template"; arg] ->
         let _ = tc.template_file <- Some (get_tc_template arg) in
         process_input argv argc (i + 1) tc
      | flag ->
         (match flag with
         | ["--help";] -> let _ = help () in tc
         | _ -> 
            let _ = err ("Unsupported flag " ^ (List.nth flag 0)) in
            tc)
     )

let unwrap (data : 'a option) : 'a =
  match data with
  | Some d -> d
  | None -> failwith "Called unwrap on None value"

let validate_tc (tc : template_creator) : unit =
  let is_valid = ref true in
  if tc.dir = None then
    let _ = print_endline "(ERR) Filepath is not supplied" in
    is_valid := false else ();
  if tc.compiler = None then
    let _ = print_endline "(ERR) Compiler is not supplied" in
    is_valid := false else ();
  if tc.template_file = None then
    let _ = print_endline "(ERR) Template is not supplied" in
    is_valid := false else ();
  if !is_valid = false then
    usage ()

let create_makefile (tc : template_creator) : unit =
  let filepath = (unwrap tc.dir) ^ "/Makefile" in
  let name = (String.split_on_char '.' (unwrap tc.name))
             |> List.rev
             |> List.tl
             |> String.concat "." in
  let content =
    name
    ^ ": "
    ^ (unwrap tc.name)
    ^ "\n\t"
    ^ (unwrap tc.compiler)
    ^ " "
    ^ (unwrap tc.name) in
  write_to_file filepath content ((unwrap tc.dir) ^ "/Makefile")

let create_template (tc : template_creator) : unit =
  let filepath = (unwrap tc.dir) ^ "/" ^ (unwrap tc.name) in
  write_to_file filepath (file_to_string (unwrap tc.template_file)) ((unwrap tc.dir) ^ "/" ^ (unwrap tc.name))

let () =
  let argc = (Array.length Sys.argv) - 1 in
  let argv = Array.sub Sys.argv 1 argc in
  let tc : template_creator = process_input argv argc 0
                                { name = None
                                ; dir = None
                                ; compiler = None
                                ; template_file = None
                                } in
  let _ = validate_tc tc in
  let _ = create_makefile tc in
  create_template tc
