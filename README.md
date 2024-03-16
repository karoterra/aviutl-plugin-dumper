# aviutl-plugin-dumper

[AviUtl](http://spring-fragrance.mints.ne.jp/aviutl/)
のプラグインファイルの情報を出力するツール。

## 使用例

コマンドライン引数にプラグインファイルを指定してください。
```cmd
> aviutl_plugin_dumper.exe path/to/exedit.auf
```

複数のプラグインファイルを一度に指定することもできます。
```cmd
> aviutl_plugin_dumper.exe exedit.auf exedit.aui exedit.auo
```

JSON、JSON Lines形式にも対応しています。
```cmd
> aviutl_plugin_dumper.exe -f json exedit.auf
> aviutl_plugin_dumper.exe -f jsonl exedit.auf
```

出力エンコーディングはShift-JISではなくUTF-8です。
そのためPowerShell等で利用する際にエンコーディングを切り替える必要がある場合があります。
```powershell
> [System.Console]::InputEncoding = [System.Text.Encoding]::UTF8
> [System.Console]::OutputEncoding = [System.Text.Encoding]::UTF8
> aviutl_plugin_dumper.exe -f json exedit.auf | ConvertFrom-Json
```

その他のオプションはヘルプを確認してください。
```cmd
> aviutl_plugin_dumper.exe -h
```

## ライセンス

[MIT License](LICENSE) に基づきます。

また、以下のライブラリを使用しています。

### aviutl_exedit_sdk v1.2

https://github.com/ePi5131/aviutl_exedit_sdk

```
Copyright (c) 2022
ePi All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
THIS SOFTWARE IS PROVIDED BY ePi “AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ePi BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```

### CLI11: Command line parser for C++11

https://github.com/CLIUtils/CLI11

```
CLI11 2.2 Copyright (c) 2017-2024 University of Cincinnati, developed by Henry
Schreiner under NSF AWARD 1414736. All rights reserved.

Redistribution and use in source and binary forms of CLI11, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its contributors
   may be used to endorse or promote products derived from this software without
   specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```

### json

https://github.com/nlohmann/json

```
MIT License

Copyright (c) 2013-2022 Niels Lohmann

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

* * *

The class contains the UTF-8 Decoder from Bjoern Hoehrmann which is licensed under the [MIT License](https://opensource.org/licenses/MIT) (see above). Copyright &copy; 2008-2009 [Björn Hoehrmann](https://bjoern.hoehrmann.de/) <bjoern@hoehrmann.de>

The class contains a slightly modified version of the Grisu2 algorithm from Florian Loitsch which is licensed under the [MIT License](https://opensource.org/licenses/MIT) (see above). Copyright &copy; 2009 [Florian Loitsch](https://florian.loitsch.com/)

The class contains a copy of [Hedley](https://nemequ.github.io/hedley/) from Evan Nemerson which is licensed as [CC0-1.0](https://creativecommons.org/publicdomain/zero/1.0/).

The class contains parts of [Google Abseil](https://github.com/abseil/abseil-cpp) which is licensed under the [Apache 2.0 License](https://opensource.org/licenses/Apache-2.0).

## 更新履歴

### v0.1.0 (2024-03-17)
- 初版
