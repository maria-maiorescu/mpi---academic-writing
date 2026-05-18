# Header: 
- Title (suggestive)
- Author block:
  - name: Maria Maiorescu
  - affiliation/address: faculty of Informatics, Dept. of Computer Science, West University of Timisoara
  - email: maria.maiorescu06@e-uvt.ro 
- Abstract: self contained, informal, short (2-3 paragraphs); contains: description of problem, domain, method(solution)

# Chapters:

1) introduction:
- typically, language is informal
- motivation: 
  - informal description of problem and domain
  - argue why the problem is relevant/ important
  - describe *existing solutions* and their limitations
  - outline/ plan for your solution
- recommended: examples:
  - simple
  - if suitable: "running example"
- declaration of originality (not a title):
  - a paragraph where the author declares their contribution to the work
- reading instructions: 
  - a paragraph (or so) in which the author explains the organization of the paper

2) formal presentation of problem and solution (not a title):
* readers: "mathematicians"
- using some formal language (syntax, semantic, reasoning system), explicit
- problem/ domain
- solution (eg. algorithmic)
- properties (proofs) of the: problem, solution (eg: correctness, efficiency)
- optional: illustrate the final presentation using running example

3) modeling of problem/ implementation of solution (not a title):
* readers: "programmers"
- programming manual:
  - architecture
  - data structure
  - etc.
* readers: "users": 
- user manual 

4) case study/ experiment (maybe a title):
- how to set data
- how do you measure
- methodology for analysis
- present results (graphics/ tables)

5) related work (ok as a title):
- comparison of related results:
  - advantages
  - disadvantages
  - advantages of own results
  - write other results 
- problem
- domain
- method

6) conclusions and future work (ok as title):
- summary of: problem, context, method:
  - What was achieved?
  - What were the problems encountered?
- future work (see problems):
  - Were they solved?:
    - yes: How?
    - no: Why?; Can you solve them?: 
      - yes: How?
      - no: Idea?

7) bibliography:
- a list of valid sources + *explicit external justification* for the role of every source in the work that was presented
- show them as item (label - numerical alphanumerical)
- complete bibliographical information

# General rules:
- choose the best available source for citations
- always give credit to the original contribution (even indirectly)

More requirements:
- add graphics - for each type of sorting algorithm (one graphic that contains all the algorithms) - and use some symbols on the graphics (for the different types of sort) so that it can be understood even if the reader hasn't colors n the screen
- tables to be able to check the correct timing (much space occupied) - add a summary table with the complexity (chapter algorithmic complexity)
- replace the C code with pseudocode (make it really easy to understand it - so that even a 5 year old can understand it) - keep it short and move it to appendix
- simplify much the user manual - remove the code there and explain in short and simpler that section
- related work - search what others did about this subject (search similar research papers on: easychair and google scholar) -> include advantages & disadvantages and add them to the bibliography (references)
- for the references use bibTex
- future work - include what we achieved and what not (if there are any problems, mention them and if you have any idea(s) on how to solve them (even if you won't solve them in the future)), examples:
-- add more data types (strings, lists, etc)
-- run the tests against other computer, with different CPU/memory to see which one influences more the results 