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
