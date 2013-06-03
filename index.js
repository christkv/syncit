var ConsoleRepl = require("./lib/console_repl").ConsoleRepl;

var options = {
    shell: false
  , nodb: false
  , norc: false
  , quiet: false
  , port: 27017
  , host: "localhost"
  , eval: null
  , username: null
  , password: null
  , authenticationMechanism: null
  , help: false
  , version: false
  , verbose: false
  , ipv6: false
  , ssl: false
  , sslCAFile: null
  , sslPEMKeyFile: null
  , sslPEMKeyPassword: null
}

var console_repl = new ConsoleRepl(options);
console_repl.start();