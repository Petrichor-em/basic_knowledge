**1**
Every node is either red or black.
**2**
All NIL nodes (figure 1) are considered black.
**3**
A red node does not have a red child.
**4**
Every path from a given node to any of its descendant NIL nodes goes through the same number of black nodes.
**5**
(Conclusion) If a node N has exactly one child, it must be a red child, because if it were black, its NIL descendants would sit at a different black depth than N's NIL child, violating requirement 4.
