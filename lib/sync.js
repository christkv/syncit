var SyncExt = require('../build/Release/sync').Sync;

var Sync = function() {  
  this._sync = new SyncExt();
}

Sync.prototype.execute = function(func, context) {
  var args = Array.prototype.slice.call(arguments, 0);  
  return this._sync.execute.apply(this, args);
}

Sync.prototype.wrap = function(func, context) {
  var self = this;

  return function() {
    var args = Array.prototype.slice.call(arguments, 0);
    context = !context ? {} : context;
    args = [func, context].concat(args);
    return self.execute.apply(self, args);
  }
}

exports.Sync = Sync;