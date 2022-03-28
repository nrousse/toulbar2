###############################################################################
# 
#         How to use toulbar2/pytoulbar2     Docker image of github Packages
#         How to use toulbar2/pytoulbar2plus Docker image of github Packages
# 
#                    ghcr.io/toulbar2/toulbar2/pytoulbar2:master
#                    ghcr.io/toulbar2/toulbar2/pytoulbar2plus:master
# 
###############################################################################

# Prerequisite ----------------------------------------------------------------

  Docker installed 

# Call pytoulbar2 -------------------------------------------------------------
 
  - problem.py : python code using pytoulbar2
 
  - call_pytoulbar2.sh : script to launch "python problem.py"
                         and install python libraries (numpy)

  - Command :

    docker run -v $PWD:/WORK -ti ghcr.io/toulbar2/toulbar2/pytoulbar2:master /bin/bash /WORK/call_pytoulbar2.sh
 
# Call pytoulbar2plus ---------------------------------------------------------
 
  - problem.py (idem)
 
  - call_pytoulbar2plus.sh : script to launch "python problem.py" (python
                             libraries already installed into Docker image)
  - Commands :

    docker run -v $PWD:/WORK -ti ghcr.io/toulbar2/toulbar2/pytoulbar2plus:master /bin/bash /WORK/call_pytoulbar2plus.sh

###############################################################################

