# GitTools
A library and set of tools for managing GitHub beyond the Git command

GitHub (and perhaps other servers) has a REST interface. It looks very GraphIQL in nature. This begins to define both an SDK for working with it as well as a few tools that make use of that SDK.

# Requirements
Requires: `git@github.com:jplflyer/ShowLib.git`. See the directions inside ShowLib to make and install locally.

Also requires curlpp, which of course has its own requirements.

# Building
Uses GNU Make and requires G++ 8 or newer. Older versions of G++ might work, but they must be G++ 17 or better.

To build, first build and install ShowLib, then:

    make
    sudo make install # optional

# Current Status
I implemented enough for my current needs, which was to list repos, teams, and users, plus I created an "Add Admin" action that lets me mark someone as an administrator of a repo.

# Usage
You'll need an API Access Token:

https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/creating-a-personal-access-token

Then you can either use the command line args or you can set ENV vars:

    export GIT_USER=foofoo
    export GIT_TOKEN=YourToken from the above

And then:

    bin/GitTool --help

To set the admin:

    bin/GitTool --org Your organization name --add-admin --repo Repository Name --login The person to add

Yes, it's very specific, but it fits the need I had.

# Contributing
The library isn't remotely complete. I did the parts I needed. You can look at Repository.h, Team.h and User.h -- which is about all I did, plus the calls available in Server.h.

If you have a particular need, you can email me, and I might just do it for you: jpl at showpage dot org. Or you can fork and edit. Adding new things it can read (gists or whatever) involves creating a new class, and look at the existing ones. It takes me 10 or 15 minutes per object type. It's just a lot of boilerplate cut + paste.

New server actions go in Server. Accessing them can either happen via GitTool.cpp or create your own tool and add it to the Makefile. Just find references to GitTool near the bottom and duplicate/edit. All you should have to do is add it to the list of bins to produce and then copy/paste/edit the link line from GitTool.

## Most Pressing
We'd need the remaining object types. See the GitHub API docs for a list, then look for a call that retrieves one of those. It will show you the JSON, and I just take that JSON and turn it into an object representation.

Next up would be implementing any Getters.

After that, we could use actions such as Create Repo, Remove Collaborator, or the like.
