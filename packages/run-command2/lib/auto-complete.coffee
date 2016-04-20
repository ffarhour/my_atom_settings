{BufferedProcess} = require 'atom'

module.exports =
class AutoComplete

  @complete: (input, cwd, options) ->
    @process = new BufferedProcess(@params(input, cwd, options))

  @params: (input = "/", cwd, options) ->
    command: if atom.config.get("run-command2.shellCommand")? then atom.config.get("run-command2.shellCommand") else '/bin/bash'
    args: ['-c', "compgen -#{options} #{input}", '-il']
    options:
      cwd: cwd
