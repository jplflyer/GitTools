# GitTools
A library and set of tools for managing GitHub beyond the Git command

GitHub (and perhaps other servers) has a REST interface. It looks very GraphIQL in nature. This begins to define both an SDK for working with it as well as a few tools that make use of that SDK.

# Requirements
Requires: `git@github.com:jplflyer/ShowLib.git`. See the directions inside ShowLib to make and install locally.

# Building
Uses GNU Make and requires G++ 8 or newer. Older versions of G++ might work, but they must be G++ 17 or better.

To build, first build and install ShowLib, then:

    make
    sudo make install # optional
