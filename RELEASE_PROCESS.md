# USBGuard Release Process

1. Clean up any artifacts from your local repository.

```
$ git clean -xfd
```

2. Setup build directory.

```
$ ./autogen.sh
$ mkdir build && cd build
$ ../configure --enable-full-test-suite --with-crypto-library=gcrypt --with-ldap
$ make
```

3. Run the test suite. All the tests must pass.

```
$ sudo make check
```

4. Compare previous lib version with current.

```
$ abidiff --stat ../abi-ref/libusbguard.so.abi .libs/libusbguard.so
$ cd ..
```

5. Update the lib version (CURRENT, REVISION, AGE) in `configure.ac` according to comments.

6. Update ABI reference.

```
$ abidw --out-file abi-ref/libusbguard.so.abi build/.libs/libusbguard.so
$ sudo rm -rf build
```

7. Update the `VERSION` file and `CHANGELOG.md`. Commit your changes.

8. Build the final release tarball.

```
$ ./autogen.sh
$ mkdir build; cd build
$ ../configure --with-crypto-library=gcrypt
$ make dist
```

9. Tag the release with a signed tag.

```
$ git tag -s -m "usbguard-0.x.y" usbguard-0.x.y
$ git push origin usbguard-0.x.x
```

10. Hash and sign the release.

```
$ sha256sum usbguard-0.x.y.tar.gz > usbguard-0.x.y.tar.gz.sum
$ gpg --armor --detach-sign usbguard-0.x.y.tar.gz
$ gpg --clearsign usbguard-0.x.y.tar.gz.sum
```

11. Create a new GitHub release using the associated tag; add the relevant section from CHANGELOG.md. Upload:

    * usbguard-0.x.y.tar.gz
    * usbguard-0.x.y.tar.gz.asc
    * usbguard-0.x.y.tar.gz.sum
    * usbguard-0.x.y.tar.gz.sum.asc
