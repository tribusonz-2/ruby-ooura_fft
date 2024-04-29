# frozen_string_literal: true
require 'mkmf'

have_header('windows.h')
have_header('pthread.h')

# 大浦先生のFFTライブラリ用設定．スレッドのコンテキストスイッチ．
have_func('pthread_create', 'pthread.h')
have_func('pthread_join', 'pthread.h')
have_func('CreateThread', 'windows.h')
have_func('WaitForSingleObject', 'windows.h')
have_func('CloseHandle', 'windows.h')

# ispow2関数．C2X標準ライブラリに取り込まれるかもしれないので登録．
have_header('stdbit.h')
have_func('ispow2l', 'stdbit.h')

create_makefile('fft')
