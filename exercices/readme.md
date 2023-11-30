# Kokkos training exercise

## environment

All exercise will be done on the `kraken` supercomputer.

Additionnal practical information that may or not be useful on site:

1. We will use git to clone some repository from github; if you need, here are some minimal infomation to create a ssh key to simplify accessing github:
```shell
# from your home on kraken
cd .ssh
mkdir github; cd github
ssh-keygen -t ed25519 -C "pierre.kestener@cea.fr" -f $HOME/.ssh/github/id_ed25519
```
2. You can then upload the public key `id_ed25519.pub` to your github account (see https://github.com/settings/keys).
3. In order to use the key, edit file `$HOME/.ssh/config` and add section like this (just change field User with your actual github username):
```text
Host github.com
    HostName github.com
    User pkestene
    PreferredAuthentications publickey
    IdentityFile ~/.ssh/github/id_ed25519
```
4. Edit file `$HOME/.gitconfig` and add minimal configuration:
```text
[user]
        name = Pierre Kestener
        email = pierre.kestener@cea.fr
[merge]
    tool = kdiff3
    ff = no
[push]
    default = simple
[pull]
    rebase = true
[core]
    editor = emacs
```

You are now good to go cloning a github repo and doing the exercise.
