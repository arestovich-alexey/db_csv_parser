# db_csv_parser
Parse csv db by index

# Usage
```bash
USAGE:

   db_parse_items_0.1_clang_x64  [-h] [--version] [-n <string>] [-s
                                 <string>] -a <string> ... -k <string> ...
                                 files ...


Where:

   -s <string>,  --spliter <string>
     spliter of string

   -n <string>,  --new_spliter <string>
     new_spliter of string

   -k <string>,  --keys <string>  (accepted multiple times)
     (required) keys of indexes

   -a <string>,  --values <string>  (accepted multiple times)
     (required) indexes to parse separated by ','. Example: -a '0,1'

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.

   files (accepted multiple times) <path>
     (required) file names```
