# Git triggers

This is an edited copy of the e-mail sent to VJ on 2020-07-08:

Here's the hooks management tool I mentioned during our call: https://github.com/rycus86/githooks. What I'm looking for in a hooks management tool:
1.	No external dependencies. Bash is perfect in that regard.
2.	One file per check, to make it easier to reason about each script and disable/enable them.
3.	Support for checks located in a repo external to the main source code one (to separate concerns).

There are other tools, but they usually fail #1 or #3.

The triggers themselves need to be written in Bash or in Python 3.5. External dependencies (especially Python modules) are to be discussed - we have some usual suspects already installed.

As discussed, I want developers to be able to run the same scripts that the server will run.
-	There should be a way to switch between enforcement and notification. On the server side it would run in enforcement, but users should be free to choose between the two (at their own peril). This can be done for example using `git config calibre.enforce-triggers 1`. We're already using this mechanism to store Calibre-specific configs related to the Git flow.
-	The second thing is that the same scripts will not run with the same Git hook. For example, the pre-receive or update hooks on the server would correspond to the pre-commit one on the client side.

This is the Jira Epic (internal server, VPN access required) with all the triggers: http://caljira.wv.mentorg.com/browse/GITI-109. Some of them need to be further split (GITI-110 and GITI-111). GITI-146 will most probably not be needed (I'm trying to get rid of LFS), and GITI-116 is low priority and requires access to an additional internal-only tool.

Three categories of triggers that can be done already:
1. CVS-related:
   - GITI-124: Disallow the use of RCS keywords. More trouble than it??s worth in CVS-land, and not used by Git, so the information will be obsoleted. This is essentially running ??grep -e '\$Id:' -e '\$Revision:' -e '\$Header:' -e '\$Log:'?? on the file contents. If the output of grep is not empty, it is a failure.
2. Build-system:
   - GITI-196: prevent files with Error (case insensitive) in their name. For example, something like `ErrorsWarningsPreferences.qml` (in the archive) is not allowed.
   - GITI-121: prevent files with mixed Unix/DOS EOL (e.g. a mix of \n and \r\n line endings). This is `file-with-mixed-endings.txt` in the attached archive.
   - GITI-122: prevent generated files from being included. Right now these are Python compiled (`*.pyc`) and encrypted (`*.pye`) files, as well as QT-generated files which have `??WARNING! All changes??` in the leading block comment. I've put `qt-generated-file.qml` in the archive as an example.
   - GITI-123: disallow `Isrc` or `Dsrc` in the path of C++ `#include` directives. These are Calibre-build specific paths that are dynamically created. For example, `lithas/main_src/Isrc/cwb/include.h` is not allowed. The file `bad-include-path.cpp` in the archive has bad and good examples.
   - GITI-112: check that all new C++ includes use the `<>` syntax. For example, `#include <subdir/include.h>` is OK, but `#include "subdir/include.h"` is not OK. Note that this is only for new includes in the commit. Pre-existing ones should be left in place. We'll get to cleaning up that technical debt later. For this, in the trigger one should run the diff with the previous file version and only check the added lines - can be looked at by running `git diff -- quotes-in-include.cpp` in the repository.
   - GITI-113: Disable checkins of certain file extensions. This could certainly be merged with GITI-122. Forbidden extensions are `.cc`, `.hh` and `.hxx`. There are three files `forbidden-extension.{cc,hh,hcc}` in the archive.
   - GITI-115: Limit file size of checked-in files. Not sure we need that one (except with a fairly large upper limit), since we intended to allow large files through LFS. Now that use of LFS is probably going away, we still have large files in the repository.
   - GITI-142: Check that only spaces, not tabs, are used in the Makefiles. This is more complicated than it seems, because tabs are only allowed to specify target commands, and not for anything else. It's the "anything else" that we need to check. Our current check only warns about tabs in new commits, but it's up to the recipient of the check's e-mail to verify that tabs are in the right place. Not sure how feasible it is to reliably implement this check. I've put `makefile-with-bad-tabs.mk` in the archive for that.
   - GITI-94: Check that all files have a final EOL when pushing. That's the way ASCII files should be, but some editors do not enforce this by default, and that causes weird error messages with some compilers. This needs to be skipped for files that Git considers binary. There's an example ASCII file `no-eol-at-eof-bad.tcl` - this should be rejected. There's a PNG file `no-eol-at-eof-ok.png` - that is OK and should not be rejected.
3. Git-related:
   - GITI-114: Check that the author/committer e-mail is of the `<person>@mentor.com` format, since Git is free-format and does not enforce anything here. We might need to extend the specification further to check that `<person>` is a valid MGC login. Also needs to be flexible so we can change/add `siemens.com` (and potentially `@splm.siemens.com`) as an authorized e-mail format in the future.
   - GITI-146: Reject commits with LFS-managed files not committed through LFS. This is to prevent people from being too smart for their own good. Again, we will most probably not use LFS in the near future, so this is low priority.

I think we can leave GITI-115 and GITI-146 in the scope as potential items, but don't implement them unless I give the go-ahead.

## Expected output from each trigger

If a trigger detects a problem in a file, it should clearly output what the problem is, and it what file the problem is. Developers should not have to waste time figuring out what the problem is.

