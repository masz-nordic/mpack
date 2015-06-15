Import('env', 'CPPFLAGS', 'LINKFLAGS')

srcs = env.Object(env.Glob('src/mpack/*.c') + env.Glob('test/*.c'),
        CPPFLAGS=env['CPPFLAGS'] + CPPFLAGS)

prog = env.Program("mpack-test", srcs,
        LINKFLAGS=env['LINKFLAGS'] + LINKFLAGS)

env.Default(env.AlwaysBuild(env.Alias("test",
    [prog],
    "valgrind --leak-check=full --errors-for-leak-kinds=all --show-leak-kinds=all " +
            "--suppressions=tools/valgrind-suppressions --error-exitcode=1 " +
            Dir('.').path + "/mpack-test")))

