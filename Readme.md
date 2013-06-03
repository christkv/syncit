SyncIt Synchronize your Node code
=================================

I was working on a proof off concept to a possible replacement for for mongo shell using node.js and one of the requirements was to run existing code of the type.

```javascript
t = db.getCollection( "basic1" );
t.drop();

o = { a : 1 };
t.save( o );

assert.eq( 1 , t.findOne().a , "first" );
assert( o._id , "now had id" );
assert( o._id.str , "id not a real id" );

o.a = 2;
t.save( o );

assert.eq( 2 , t.findOne().a , "second" );
assert(t.validate().valid);

// not a very good test of currentOp, but tests that it at least 
// is sort of there:
assert( db.currentOp().inprog != null );
assert(false)
```

This module is what I came up with. To use do the following.

```javascript
var Sync = require('sync');
// Execute an async method as sync
var result = new Sync().execute(fs.readFile, fs, __dirname + "/file_read_test.js");
// Result is an object of type {err:.., result:..} mirroring the 
// callback structure function(err, result) {}
```

