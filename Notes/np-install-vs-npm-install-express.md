### What Happens During `npm install express`
npm will:

1. connect to the npm registry
2. download the package
3. download its dependencies
4. check for `node_modules`

   - if `node_modules` does not exist → npm creates it
   - if it already exists → npm reuses/updates it

5. install packages inside `node_modules`
6. update `package.json`
7. create/update `package-lock.json`




npm first connects to the npm registry.

```txt
https://registry.npmjs.org/
```

The npm registry is an online database that stores Node.js packages.

npm searches for:

```txt
express
```


### What Happens During `npm install`

npm will:

- read `package.json`
- check `package-lock.json`

If exact versions are present in `package-lock.json`:

- npm installs those exact versions

npm then checks:

```txt
node_modules/
```

If `node_modules` does not exist:

- npm creates it

If required packages already exist there with correct versions:

- npm skips downloading and reinstalling them

Otherwise npm:

- checks npm cache

If package files already exist in cache:

- npm installs packages from cache into `node_modules`

Otherwise npm:

- connects to the npm registry
- downloads package files and dependencies
- stores them in cache
- installs them into `node_modules`

Finally npm:

- creates/updates `package-lock.json`

Result:

- dependencies become available locally
- the project becomes runnable










