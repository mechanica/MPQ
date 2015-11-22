mech-mpq
==========

A StormLib MPQ wrapper for Node.js.

## Installation
`npm install mech-mpq`

## Usage
### Example
Open an MPQ archive and read a file:
```
var mpq = require('mech-mpq');

function getFileContents(mpqPath, filePath) {
    var archive = mpq.openArchive(mpqPath);
    if (archive) {
        var file = archive.openFile(filePath);
        if (file) {
            var fileContents = file.read();
            file.closeFile();
            return fileContents;
        }
    }
    return null;
}

var fileContents = getFileContents('./path/to/archive.mpq', 'filename.txt');
if (fileContents) {
    console.log(fileContents.length);
    console.log(fileContents);
}
```

See the [tests](https://github.com/mechanica/MPQ/tree/master/test) for more examples.

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
