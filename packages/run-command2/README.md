# "Run Command" for Atom
Fork of https://github.com/kylewlacy/run-command
Execute any arbitrary command in [Atom](http://atom.io). Derived from Phil Nash's [Ruby Quick Test](https://github.com/philnash/ruby-quick-test).

# Run Any command!
### Grunt
!['Run Command' dialog](https://raw.githubusercontent.com/nathanjohnson320/run-command2/master/screenshots/grunt.png)

### Color Support
!['Run Command' dialog](https://raw.githubusercontent.com/nathanjohnson320/run-command2/master/screenshots/colors.png)

### Bash Completion
Integrated with compgen to give you tab autocompletion.

!['Run Command' dialog](https://raw.githubusercontent.com/nathanjohnson320/run-command2/master/screenshots/bash_support.png)

Hit tab and you'll get the largest substring filled in for you.
!['Run Command' dialog](https://raw.githubusercontent.com/nathanjohnson320/run-command2/master/screenshots/auto_complete.png)

# Usage

Make sure you have ~/.bashrc setup so your path works properly

Smack `ctrl-r` to open up this:

!['Run Command' dialog](https://raw.githubusercontent.com/kylewlacy/run-command/master/screenshots/run-command.gif)

Enter a command, and whack `enter` to run it:

![Running `rake spec`](https://raw.githubusercontent.com/kylewlacy/run-command/master/screenshots/run.gif)

Clack down `cmd-ctrl-r` to run it again:

![Re-running `rake spec`](https://raw.githubusercontent.com/kylewlacy/run-command/master/screenshots/re-run.gif)

Together, these let you do this:

![Bernhardt-style TDD](https://raw.githubusercontent.com/kylewlacy/run-command/master/screenshots/tdd.gif)

(You can also toggle the command output with `cmd-ctrl-x`, or kill the last command with `cmd-ctrl-alt-x`)

# TODO
- ~~ANSI color codes~~
- ~~Setting the working directory~~
- ~~Tab autocompletion~~
- ~~Copy paste from output~~
- ~~Resizable output~~
- Editor variables (such as `$ATOM_PROJECT` for the current project directory)
