# Skylark rule to generate a Junit4 TestSuite
# Assumes srcs are all.java Test files
# Assumes junit4 is already added to deps by the user.

_OUTPUT = """import org.junit.runners.Suite;
    import org.junit.runner.RunWith;

@RunWith(Suite.class)
@Suite.SuiteClasses({ %s})
public class %s {
}
"""

_DIRS = ("src", "javatests", "java")
_PREFIXES = ("org", "com", "edu")

def _SafeIndex(l, val):
  for i, v in enumerate(l):
    if val == v:
      return i
  return -1

def _CodeRootIndex(toks):
  for s in _PREFIXES:
    findex = _SafeIndex(toks, s)
    if findex != -1:
      return findex
  for s in _DIRS:
    findex = _SafeIndex(toks, s)
    if findex != -1:
      return findex + 1 # assume code starts at the next level
  fail("%s does not contain any of %s or %s",
       toks, _PREFIXES, _DIRS)

def _AsClassName(fname):
  fname = [x.path for x in fname.files][0]
  toks = fname[:-5].split("/") # remove .java and dir-split
  findex = _CodeRootIndex(toks)
  return ".".join(toks[findex:]) + ".class"

def _junit_tests_impl(ctx):
  classes = ",".join(
      [_AsClassName(x) for x in ctx.attr.srcs])
  ctx.file_action(output=ctx.outputs.out, content=_OUTPUT % (
      classes, ctx.attr.outname))

_GenSuite = rule(
  implementation=_junit_tests_impl,
  attrs={
    "srcs": attr.label_list(allow_files=True),
    "outname": attr.string(),
  },
  outputs={"out": "%{name}.java"},
  output_to_genfiles = True,
)

def junit_tests(name, srcs, **kwargs):
  s_name = name + "TestSuite"
  _GenSuite(name=s_name,
            srcs=srcs,
            outname=s_name)
  native.java_test(name=name,
                   test_class=s_name,
                   srcs = srcs + [":"+s_name],
                   **kwargs)
