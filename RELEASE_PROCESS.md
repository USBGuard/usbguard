# USBGuard Release Process

1. Clean up any artifact from your local repository.

```
$ git clean -x -f -d
```

2. Run the test suite. All the tests must pass.

```
$ ./autogen.sh
$ mkdir build; cd build
$ ../configure --enable-full-test-suite
$ make check
$ cd ..; rm -rf build
```

3. Update the `VERSION` file and `CHANGELOG.md`. Commit your changes.

4. Build the final release tarball.

```
$ ./autogen.sh
$ mkdir build; cd build
$ ../configure
$ make dist
```

5. Tag the release with a signed tag.

```
$ git tag -s -m "usbguard-0.x.y" usbguard-0.x.y
$ git push origin usbguard-0.x.x
```

6. Hash and sign the release.

```
$ sha256sum usbguard-0.x.y.tar.gz > usbguard-0.x.y.tar.gz.sum
$ gpg --armor --detach-sign usbguard-0.x.y.tar.gz
$ gpg --clearsign usbguard-0.7.5.tar.gz.sum
```

7. Create a new GitHub release using the associated tag; add the relevant section from CHANGELOG.md. Upload:

    * usbguard-0.x.y.tar.gz
    * usbguard-0.x.y.tar.gz.asc
    * usbguard-0.x.y.tar.gz.sum
    * usbguard-0.x.y.tar.gz.sum.asc

