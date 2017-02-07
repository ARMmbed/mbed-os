# Build Profiles
Mbed 5.0 supports three primary build profiles, *develop*, *debug* and *release*. When using
the online compiler the *develop* profile is used. When building from the command line
the desired profile can be can be selected by adding the ```--profile <profile>```
command line flag, but `develop` will be used by default. Custom user defined profiles can also be specific by giving the path
the the profile.

## Develop profile
* Small and fast code
* Full error information - e.x. asserts have filename and line number
* Hard to follow code flow when using a debugger

## Debug profile
* Easy to step through code with a debugger
* Full error information - e.x. asserts have filename and line number
* Largest and slowest profile

## Release profile
* Smallest profile and still fast
* Minimal error information
* Hard to follow code flow when using a debugger
