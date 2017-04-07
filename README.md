# Prime Finder

A simple multi-threaded program that finds primes

## Installation

Please use `<make> .\bin\make.exe` to compile the program. You can also use `<make> all`.

## Usage

Run the excutable with these parameters:

| Parameter | Default |                                                                  |
|-----------|---------|------------------------------------------------------------------|
| -t        | 4       | Use this argument to specify number of threads                   |
| -m        | 100000  | Use this argument to specify the maximum number                  |
| -r        |         | Use this argument to specify a specific number                   |
| -n        |         | Use this argument to specify the maximum number                  |
| -d        | False   | Use this switch to specify whether to display the bitmap         |
| -dall     |         | Use this switch to specify whether to primes from all threads    |

Example: `.\PrimeFinder.exe -t 7 -m 500000 -d -dall`
  
Example: `.\PrimeFinder.exe -t 4 -r 500000000`
  
Example: `.\PrimeFinder.exe -t 7 -e 300000 -d`
  
## Contributing

1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request :D

## License

Copyright (c) 2017 The Gupta Empire - All Rights Reserved

Copyright (c) 2017 Aryan Gupta
