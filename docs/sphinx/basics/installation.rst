Installation
============
Currently only abstractions for Unix has been written. So if you are using either a windows machine then you will have to settle for using this in a docker environment.


Linux
-----
1.  Clone the repo ``georgeshanti/XSe.git``

    .. code-block:: shell-session

        $ git clone https://github.com/georgeshanti/XSe.git

2.  Build from sources files. This will compile it all down to a ``libxse.a``

    .. code-block:: shell-session

        $ cd XSe
        $ cmake .
        $ make xse

3.  Install binary and the header files to the system. This is copy all the ``.hpp`` files to ``/usr/`` and ``libxse.a`` to ``/usr/lib/``

    .. code-block:: shell-session

        $ make install

Docker
------
Sorry that this is the only other option. Build with the included ``Dockerfile`` and run ``env.sh``