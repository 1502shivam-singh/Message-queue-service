# Contribution

Steps to contribute

- Fork this repository and clone down your copy in your system. 

- Setting the proper upstream. Switch to your forked master branch, pull codes from upstream, then create a new branch.

      $ git remote add upstream https://github.com/1502shivam-singh/Message-queue-service.git
      $ git checkout master
      $ git pull upstream master
      $ git checkout -b IssueNo


- As the service is written using the Winsock2 library, Windows would be a better OS for running this.
For BSD OSs compatibility, some basic porting will be required (Mostly for WSA... type functions)

- After coding, then push codes to your fork repo.

      $ git add modified-file-names
      $ git commit -m 'commit message'
      $ git push origin issueNo

- Finally submit a PR
