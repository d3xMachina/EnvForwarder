# EnvForwarder

Forwarder that allows you to change environment variables before starting a process with support for regular expressions syntax.

## How to use

- Download the release or compile with VS Studio 2022.
- If you want to forward to a console application use EnvForwarderCLI.exe as it will also forward the console pipes.
- Make a folder "EnvForwarder" where "EnvForwarder.exe" is located.
- Inside this folder, create a file "path.txt" containing the path to the application you want to change the environment variables (example: "c:\Apps\myapp.exe" without the quotes)
- Replace the calls to your application with this one (see the helper folder for an example) **OR** rename your application to something else and EnvForwarder.exe to your original application name (make sure to fix the path in path.txt in this case).
- Add your rules by adding a file "rules.txt" inside the "EnvForwarder" folder. (check [Rules Syntax](#rules-syntax))
- Start the EnvForwarder executable instead of your application executable.
- Profit

Logs are stored in EnvForwarder\logs.log where EnvForwarder is located.

## Rules Syntax

The syntax of the environment variables rules in "rules.txt" is :
```
ENVIRONMENT_VARIABLE OPERATION REGEXP
```

Each rule is on a line. There are three operations "APPEND", "PREPEND" and "REMOVE". You can do multiple operations on a single environment variable.
For the regexp syntax check https://cplusplus.com/reference/regex/ECMAScript/

Example of rules removing python from the environment variables to use another version :
```
PATH REMOVE ;?[^;]*\\AppData\\Local\\Programs\\Python\\[^;]*
PATH PREPEND C:\\MyApps\\Python310
PATH PREPEND C:\\MyApps\\Python310\\Scripts
PYTHONPATH REMOVE .*
```

## License

EnvForwarder is available on Github under the MIT license.