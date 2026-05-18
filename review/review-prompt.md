# Scientific paper review rules
1. Read abstract, introduction and conclusion 
2. if the paper is in the scope (use #scientificPaper.md and system_prompt.md to understand the overall scope of the assignment)
    - then report "the paper is in the scope", go to #3
    - else describe the topic of the paper, report "reject because the paper is not in the scope" - STOP
3. read the main parts of the paper, write a short summary of the paper: problem, main results
4. if the presentation of the pare is clear enough for a fair evaluation of objectives and achievements:
    - then go to 5
    - else report "the presentation is too poor for a reasonable evaluation" - STOP
5. if the main results of the paper seem important:
    - then report the main results (list them) are very/sufficiently/reasonably important. go to 6
    - else report "the main results are not important" + justify + STOP 
6. if the results are non-trivial, reasonably difficult to achieve:
    - then report "the following results (list them) are: non-trivial/difficult/ingenious to achieve + justify. go to 7
    - else report "the following results (list them) are trivial/easy/straightforward to achieve + justify. STOP
7. if the results seem to be original:
    - then report "the results seem to be new" + why?. go to 8
    - else report "reject, the results seem to be known" + justify (point to literature) + stop
8. if there are sufficiently many details for checking correctness:
    - then report and go to 9
    - else report "the paper contains to few details (which? what is missing?) to check correctness"
8.1 if by intuition, the results seem to have a high chance to be correct:
    - then report "results seem intuitively correct" + why + go to 11
    - else reject, report what results seem wrong, why + STOP
9. read the paper in detail, check correctness
10. if the paper is technically correct and the author seems to master the field:
    - then report "the author seems to master completely/sufficiently/..
        - example "I have checked completely/partially/not-at-all some/all-details + make a list of minor errors/problems.
    - else "reject, the paper contains errors" + explain the errors + stop
11. if the presentation, structure, language, style, format are of high standard:
    - then report, go to 12
    - else report problems, make a lists of suggestions for improving the presentation, STOP
12. make a list of suggestions for improving the paper, make your recommendation / proposals .

