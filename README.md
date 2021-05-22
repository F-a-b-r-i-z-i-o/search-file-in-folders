# search-folder
custom_find command program that recursively searches for files with a certain extension in the subtree rooted in the directory passed as an argument (see below).
For each file found, it prints the absolute path of the directory in which it was found and the date of the last modification.
The program receives at most two input arguments:

1) extension string, e.g., ".txt" [first argument is required]
2) string path [second argument, optional]: if the argument exists, search in the subtree rooted in the "path" directory, if not there, search starting from "."
