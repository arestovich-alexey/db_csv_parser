# db_csv_parser
Parse csv db by index

# Usage
```bash
USAGE:

   db_parse_0.1.1_Os  [-hu] [--version] [-n <string>] [-s <string>] -a
                      <string> ... -k <string> ... files ...


Where:

   -s <string>,  --spliter <string>
     spliter of string

   -n <string>,  --new_spliter <string>
     new_spliter of string

   -u,  --unescape
     Unescape spliters. To work with '\t', etc.

   -k <string>,  --keys <string>  (accepted multiple times)
     (required) keys of indexes. Example: -k key1 -k key2

   -a <string>,  --values <string>  (accepted multiple times)
     (required) indexes to parse separated by ','. Example: -a '0,1' -a '3,
     4'

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.

   files (accepted multiple times) <path>
     (required) file names
```
# Example:
```bash
db_parse -s "," -n ":" -k value1 -a 0,1 -k val2 -a 1,2 file1.txt file2.txt
```
with \t spliter, always use ```-u``` argument(for unescape):
```bash
db_parse -s "\t" -u -n ":" -k value1 -a 0,1 -k val2 -a 1,2 file1.txt file2.txt
```
