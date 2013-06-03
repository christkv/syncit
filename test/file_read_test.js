var Sync = require("../").Sync
  , fs = require('fs');

/**
 * Retrieve the server information for the current
 * instance of the db client
 *
 * @ignore
 */
exports.setUp = function(callback) {
  callback();
}

/**
 * Retrieve the server information for the current
 * instance of the db client
 *
 * @ignore
 */
exports.tearDown = function(callback) {
  callback();
}

exports['Make your file async read synchronous'] = function(test) {
  var text = fs.readFileSync(__dirname + "/file_read_test.js");
  var result = new Sync().execute(fs.readFile, fs, __dirname + "/file_read_test.js");
  test.equal(null, result.err);
  test.deepEqual(text, result.result);
  test.done();
}

exports['Read a web page synchronous'] = function(test) {
  var text = fs.readFileSync(__dirname + "/file_read_test.js");
  var result = new Sync().execute(fs.readFile, fs, __dirname + "/file_read_test.js");
  test.equal(null, result.err);
  test.deepEqual(text, result.result);
  test.done();
}
