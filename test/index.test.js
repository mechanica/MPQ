'use strict';
/*jshint expr: true*/
/*global describe, it, afterEach, beforeEach*/

var expect = require('chai').expect,
    path = require('path'),
    mkdirp = require('mkdirp'),
    rimraf = require('rimraf'),
    bufferEqual = require('buffer-equal'),
    mpq = require('../');

function getDataPath(filename) {
    filename = filename || '';
    return path.join('./test/temp/', filename);
}

describe('archive operations', function() {
    beforeEach(function() {
        rimraf.sync(getDataPath());
        mkdirp.sync(getDataPath());
    });

    afterEach(function() {
        rimraf.sync(getDataPath());
    });

    it('can create and open archives', function() {
        var archivePath = getDataPath('abc.mpq');
        var archive = mpq.createArchive(archivePath);
        expect(archive).to.exist;
        expect(archive.flush()).to.be.true;
        archive.close();
        archive = mpq.openArchive(archivePath);
        expect(archive).to.exist;
        archive.close();
    });

    it('can create, remove, and rename files within an archive', function() {
        var archivePath = getDataPath('abc.mpq');
        var filePath_0 = "foo/bar_0.txt";
        var filePath_1 = "foo/bar_1.txt";

        var archive = mpq.createArchive(archivePath);
        expect(archive).to.exist;

        expect(archive.hasFile(filePath_0)).to.be.false;
        expect(archive.createFile(filePath_0, 1024)).to.exist;

        expect(archive.hasFile(filePath_0)).to.be.true;
        expect(archive.hasFile(filePath_1)).to.be.false;
        expect(archive.renameFile(filePath_0, filePath_1)).to.be.true;
        expect(archive.hasFile(filePath_0)).to.be.false;
        expect(archive.hasFile(filePath_1)).to.be.true;

        expect(archive.removeFile(filePath_1)).to.be.true;
        expect(archive.hasFile(filePath_0)).to.be.false;
        expect(archive.hasFile(filePath_1)).to.be.false;

        archive.flush();
        archive.close();
    });
});

describe('file operations', function() {
    beforeEach(function() {
        rimraf.sync(getDataPath());
        mkdirp.sync(getDataPath());
    });

    afterEach(function() {
        rimraf.sync(getDataPath());
    });

    function createArchive() {
        var archivePath = getDataPath('abc.mpq');
        var archive = mpq.createArchive(archivePath);
        expect(archive).to.exist;
        return archive;
    }

    function getRandomIntInclusive(min, max) {
        return Math.floor(Math.random() * (max - min + 1)) + min;
    }

    it('can create and open files within an archive', function() {
        var archive = createArchive();
        var filePath = "foo/bar/hoge";
        var fileSize = 2048;
        var file = archive.createFile(filePath, fileSize);
        expect(file).to.exist;

        expect(file.getSize()).to.equal(fileSize);

        var otherFile = archive.openFile(filePath);
        expect(otherFile).to.exist;
        expect(otherFile.getSize()).to.equal(fileSize);
        otherFile.close();

        archive.close();
    });

    it('can read and write files within an archive', function() {
        var archive = createArchive();
        var filePath = "foo/bar/hoge";
        var fileSize = 1024;

        var file = archive.createFile(filePath, fileSize);
        expect(file).to.exist;
        expect(file.getSize()).to.equal(fileSize);

        var bytes = [];
        for (var i = 0; i < fileSize; i++) {
            bytes.push(getRandomIntInclusive(0, 0xFF));
        }

        var buffer = new Buffer(bytes);
        expect(buffer.length).to.equal(fileSize);
        expect(file.write(buffer)).to.be.true;
        expect(file.close()).to.be.true;

        file = archive.openFile(filePath);
        expect(file).to.exist;
        expect(file.getSize()).to.equal(fileSize);

        var otherBuffer = file.read();
        expect(otherBuffer).to.exist;
        expect(otherBuffer.length).to.equal(fileSize);
        file.close();

        expect(bufferEqual(buffer, otherBuffer)).to.be.true;

        archive.close();
    });
});
