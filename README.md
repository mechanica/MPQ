mech-mpq
==========

A StormLib MPQ wrapper for Node.js.

## Installation
`npm install mech-mpq`

## Usage
`var mpq = require('mech-mpq');`

### Example
Open an MPQ archive and read a file:
```
try {
    var archive = mpq.openArchive('./path/to/file.mpq');
    var file = archive.openFile('filename.txt');
    var fileContents = file.readFile();
    file.closeFile();

    console.log(fileContents.length);
    console.log(fileContents);
}
catch (err) {
    console.error(err);
}
```

## Dev
### Compilation
1. Clone:
  * `git clone https://github.com/mechanica/MPQ.git`
1. Checkout submodules:
  * `git submodule init && git submodule update`
1. Build:
  * `npm install`
1. Run tests:
  * `npm test`
