"""
Sample script using MyNav API. Show a callgraph with every function
which calls the selected (at cursor) function.
"""

from idautils import CodeRefsFrom

def showCodeRefsTo(ea):
    l = list(CodeRefsTo(ea, 1))
    #l.append(ea)

    if len(l) > 0:
        g = mybrowser.PathsBrowser("Code Refs to", l, [], [])
        g.Show()
    else:
        info("No code refs to the current point!")

def targetPointsCodeRefsTo(ea):
    for p in CodeRefsTo(ea, 1):
        mynav.addPoint(ea, "T")

if __name__ == "__main__":
    showCodeRefsTo(ScreenEA())
    #targetPointsCodeRefsTo(here())
