#include <ctype.h>
#include <stdio.h>

#include "util.h"

static void Debug(val, a, b, c, d, e, f, g) int val;
char *a, *b, *c, *d, *e, *f, *g;
{}
#define _(String) (String)
int Suffix(myword, suffix) char *myword;
char *suffix;
{
    register int i;
    register int j;
    i = strlen(myword);
    j = strlen(suffix);

    if (i > j) {
        return (STRCMP((myword + i - j), suffix));
    } else {
        return (-1);
    }
}

char *Reverse(str) /* return a pointer to a reversal */
    register char *str;
{
    register int i;
    register int j;
    static char area[STRINGSIZE];
    j = i = strlen(str);
    while (*str) {
        area[--i] = *str++;
    }
    area[j] = '\0';
    return (area);
}

char *Uppercase(str) /* return a pointer to an uppercase */
    register char *str;
{
    register char *ptr;
    static char area[STRINGSIZE];
    ptr = area;
    while (*str) {
        *(ptr++) = CRACK_TOUPPER(*str);
        str++;
    }
    *ptr = '\0';

    return (area);
}

char *Lowercase(str) /* return a pointer to an lowercase */
    register char *str;
{
    register char *ptr;
    static char area[STRINGSIZE];
    ptr = area;
    while (*str) {
        *(ptr++) = CRACK_TOLOWER(*str);
        str++;
    }
    *ptr = '\0';

    return (area);
}

char *Capitalise(str) /* return a pointer to an capitalised */
    register char *str;
{
    register char *ptr;
    static char area[STRINGSIZE];
    ptr = area;

    while (*str) {
        *(ptr++) = CRACK_TOLOWER(*str);
        str++;
    }

    *ptr = '\0';
    area[0] = CRACK_TOUPPER(area[0]);
    return (area);
}

char *Pluralise(string) /* returns a pointer to a plural */
    register char *string;
{
    register int length;
    static char area[STRINGSIZE];
    length = strlen(string);
    strcpy(area, string);

    if (!Suffix(string, "ch") || !Suffix(string, "ex") || !Suffix(string, "ix") || !Suffix(string, "sh") ||
        !Suffix(string, "ss")) {
        /* bench -> benches */
        strcat(area, "es");
    } else if (length > 2 && string[length - 1] == 'y') {
        if (strchr("aeiou", string[length - 2])) {
            /* alloy -> alloys */
            strcat(area, "s");
        } else {
            /* gully -> gullies */
            strcpy(area + length - 1, "ies");
        }
    } else if (string[length - 1] == 's') {
        /* bias -> biases */
        strcat(area, "es");
    } else {
        /* catchall */
        strcat(area, "s");
    }

    return (area);
}

char *Substitute(string, old, new) /* returns pointer to a swapped about copy */
    register char *string;
register char old;
register char new;
{
    register char *ptr;
    static char area[STRINGSIZE];
    ptr = area;
    while (*string) {
        *(ptr++) = (*string == old ? new : *string);
        string++;
    }
    *ptr = '\0';
    return (area);
}

char *Purge(string, target) /* returns pointer to a purged copy */
    register char *string;
register char target;
{
    register char *ptr;
    static char area[STRINGSIZE];
    ptr = area;
    while (*string) {
        if (*string != target) {
            *(ptr++) = *string;
        }
        string++;
    }
    *ptr = '\0';
    return (area);
}
/* -------- CHARACTER CLASSES START HERE -------- */

/*
 * this function takes two inputs, a class identifier and a character, and
 * returns non-null if the given character is a member of the class, based
 * upon restrictions set out below
 */

int MatchClass(class, input) register char class;
register char input;
{
    register char c;
    register int retval;
    retval = 0;

    switch (class) {
            /* ESCAPE */

        case '?': /* ?? -> ? */
            if (input == '?') {
                retval = 1;
            }
            break;

            /* ILLOGICAL GROUPINGS (ie: not in ctype.h) */

        case 'V':
        case 'v': /* vowels */
            c = CRACK_TOLOWER(input);
            if (strchr("aeiou", c)) {
                retval = 1;
            }
            break;

        case 'C':
        case 'c': /* consonants */
            c = CRACK_TOLOWER(input);
            if (strchr("bcdfghjklmnpqrstvwxyz", c)) {
                retval = 1;
            }
            break;

        case 'W':
        case 'w': /* whitespace */
            if (strchr("\t ", input)) {
                retval = 1;
            }
            break;

        case 'P':
        case 'p': /* punctuation */
            if (strchr(".`,:;'!?\"", input)) {
                retval = 1;
            }
            break;

        case 'S':
        case 's': /* symbols */
            if (strchr("$%%^&*()-_+=|\\[]{}#@/~", input)) {
                retval = 1;
            }
            break;

            /* LOGICAL GROUPINGS */

        case 'L':
        case 'l': /* lowercase */
            if (islower(input)) {
                retval = 1;
            }
            break;

        case 'U':
        case 'u': /* uppercase */
            if (isupper(input)) {
                retval = 1;
            }
            break;

        case 'A':
        case 'a': /* alphabetic */
            if (isalpha(input)) {
                retval = 1;
            }
            break;

        case 'X':
        case 'x': /* alphanumeric */
            if (isalnum(input)) {
                retval = 1;
            }
            break;

        case 'D':
        case 'd': /* digits */
            if (isdigit(input)) {
                retval = 1;
            }
            break;

        default:
            Debug(1, "MatchClass: unknown class %c\n", class);
            return (0);
            break;
    }

    if (isupper(class)) {
        return (!retval);
    }
    return (retval);
}

char *PolyStrchr(string, class) register char *string;
register char class;
{
    while (*string) {
        if (MatchClass(class, *string)) {
            return (string);
        }
        string++;
    }
    return NULL;
}

char *PolySubst(string, class, new) /* returns pointer to a swapped about copy */
    register char *string;
register char class;
register char new;
{
    register char *ptr;
    static char area[STRINGSIZE];
    ptr = area;
    while (*string) {
        *(ptr++) = (MatchClass(class, *string) ? new : *string);
        string++;
    }
    *ptr = '\0';
    return (area);
}

char *PolyPurge(string, class) /* returns pointer to a purged copy */
    register char *string;
register char class;
{
    register char *ptr;
    static char area[STRINGSIZE];
    ptr = area;
    while (*string) {
        if (!MatchClass(class, *string)) {
            *(ptr++) = *string;
        }
        string++;
    }
    *ptr = '\0';
    return (area);
}
/* -------- BACK TO NORMALITY -------- */

int Char2Int(character) char character;
{
    if (isdigit(character)) {
        return (character - '0');
    } else if (islower(character)) {
        return (character - 'a' + 10);
    } else if (isupper(character)) {
        return (character - 'A' + 10);
    }
    return (-1);
}

char *Mangle(input, control) /* returns a pointer to a controlled Mangle */
    char *input;
char *control;
{
    int limit;
    register char *ptr;
    static char area[STRINGSIZE * 2] = {0};
    char area2[STRINGSIZE * 2] = {0};
    strcpy(area, input);

    for (ptr = control; *ptr; ptr++) {
        switch (*ptr) {
            case RULE_NOOP:
                break;
            case RULE_REVERSE:
                strcpy(area, Reverse(area));
                break;
            case RULE_UPPERCASE:
                strcpy(area, Uppercase(area));
                break;
            case RULE_LOWERCASE:
                strcpy(area, Lowercase(area));
                break;
            case RULE_CAPITALISE:
                strcpy(area, Capitalise(area));
                break;
            case RULE_PLURALISE:
                strcpy(area, Pluralise(area));
                break;
            case RULE_REFLECT:
                strcat(area, Reverse(area));
                break;
            case RULE_DUPLICATE:
                strcpy(area2, area);
                strcat(area, area2);
                break;
            case RULE_GT:
                if (!ptr[1]) {
                    Debug(1, "Mangle: '>' missing argument in '%s'\n", control);
                    return NULL;
                } else {
                    limit = Char2Int(*(++ptr));
                    if (limit < 0) {
                        Debug(1, "Mangle: '>' weird argument in '%s'\n", control);
                        return NULL;
                    }
                    if ((int)strlen(area) <= limit) {
                        return NULL;
                    }
                }
                break;
            case RULE_LT:
                if (!ptr[1]) {
                    Debug(1, "Mangle: '<' missing argument in '%s'\n", control);
                    return NULL;
                } else {
                    limit = Char2Int(*(++ptr));
                    if (limit < 0) {
                        Debug(1, "Mangle: '<' weird argument in '%s'\n", control);
                        return NULL;
                    }
                    if ((int)strlen(area) >= limit) {
                        return NULL;
                    }
                }
                break;
            case RULE_PREPEND:
                if (!ptr[1]) {
                    Debug(1, "Mangle: prepend missing argument in '%s'\n", control);
                    return NULL;
                } else {
                    area2[0] = *(++ptr);
                    strcpy(area2 + 1, area);
                    strcpy(area, area2);
                }
                break;
            case RULE_APPEND:
                if (!ptr[1]) {
                    Debug(1, "Mangle: append missing argument in '%s'\n", control);
                    return NULL;
                } else {
                    register char *string;
                    string = area;
                    while (*(string++))
                        ;
                    string[-1] = *(++ptr);
                    *string = '\0';
                }
                break;
            case RULE_EXTRACT:
                if (!ptr[1] || !ptr[2]) {
                    Debug(1, "Mangle: extract missing argument in '%s'\n", control);
                    return NULL;
                } else {
                    register int i;
                    int start;
                    int length;
                    start = Char2Int(*(++ptr));
                    length = Char2Int(*(++ptr));
                    if (start < 0 || length < 0) {
                        Debug(1, "Mangle: extract: weird argument in '%s'\n", control);
                        return NULL;
                    }
                    strcpy(area2, area);
                    for (i = 0; length-- && area2[start + i]; i++) {
                        area[i] = area2[start + i];
                    }
                    /* cant use strncpy() - no trailing NUL */
                    area[i] = '\0';
                }
                break;
            case RULE_OVERSTRIKE:
                if (!ptr[1] || !ptr[2]) {
                    Debug(1, "Mangle: overstrike missing argument in '%s'\n", control);
                    return NULL;
                } else {
                    register int i;
                    i = Char2Int(*(++ptr));
                    if (i < 0) {
                        Debug(1, "Mangle: overstrike weird argument in '%s'\n", control);
                        return NULL;
                    } else {
                        ++ptr;
                        if (area[i]) {
                            area[i] = *ptr;
                        }
                    }
                }
                break;
            case RULE_INSERT:
                if (!ptr[1] || !ptr[2]) {
                    Debug(1, "Mangle: insert missing argument in '%s'\n", control);
                    return NULL;
                } else {
                    register int i;
                    register char *p1;
                    register char *p2;
                    i = Char2Int(*(++ptr));
                    if (i < 0) {
                        Debug(1, "Mangle: insert weird argument in '%s'\n", control);
                        return NULL;
                    }
                    p1 = area;
                    p2 = area2;
                    while (i && *p1) {
                        i--;
                        *(p2++) = *(p1++);
                    }
                    *(p2++) = *(++ptr);
                    strcpy(p2, p1);
                    strcpy(area, area2);
                }
                break;
                /* THE FOLLOWING RULES REQUIRE CLASS MATCHING */

            case RULE_PURGE: /* @x or @?c */
                if (!ptr[1] || (ptr[1] == RULE_CLASS && !ptr[2])) {
                    Debug(1, "Mangle: delete missing arguments in '%s'\n", control);
                    return NULL;
                } else if (ptr[1] != RULE_CLASS) {
                    strcpy(area, Purge(area, *(++ptr)));
                } else {
                    strcpy(area, PolyPurge(area, ptr[2]));
                    ptr += 2;
                }
                break;
            case RULE_SUBSTITUTE: /* sxy || s?cy */
                if (!ptr[1] || !ptr[2] || (ptr[1] == RULE_CLASS && !ptr[3])) {
                    Debug(1, "Mangle: subst missing argument in '%s'\n", control);
                    return NULL;
                } else if (ptr[1] != RULE_CLASS) {
                    strcpy(area, Substitute(area, ptr[1], ptr[2]));
                    ptr += 2;
                } else {
                    strcpy(area, PolySubst(area, ptr[2], ptr[3]));
                    ptr += 3;
                }
                break;
            case RULE_MATCH: /* /x || /?c */
                if (!ptr[1] || (ptr[1] == RULE_CLASS && !ptr[2])) {
                    Debug(1, "Mangle: '/' missing argument in '%s'\n", control);
                    return NULL;
                } else if (ptr[1] != RULE_CLASS) {
                    if (!strchr(area, *(++ptr))) {
                        return NULL;
                    }
                } else {
                    if (!PolyStrchr(area, ptr[2])) {
                        return NULL;
                    }
                    ptr += 2;
                }
                break;
            case RULE_NOT: /* !x || !?c */
                if (!ptr[1] || (ptr[1] == RULE_CLASS && !ptr[2])) {
                    Debug(1, "Mangle: '!' missing argument in '%s'\n", control);
                    return NULL;
                } else if (ptr[1] != RULE_CLASS) {
                    if (strchr(area, *(++ptr))) {
                        return NULL;
                    }
                } else {
                    if (PolyStrchr(area, ptr[2])) {
                        return NULL;
                    }
                    ptr += 2;
                }
                break;
                /*
                 * alternative use for a boomerang, number 1: a standard throwing
                 * boomerang is an ideal thing to use to tuck the sheets under
                 * the mattress when making your bed.  The streamlined shape of
                 * the boomerang allows it to slip easily 'twixt mattress and
                 * bedframe, and it's curve makes it very easy to hook sheets
                 * into the gap.
                 */

            case RULE_EQUALS: /* =nx || =n?c */
                if (!ptr[1] || !ptr[2] || (ptr[2] == RULE_CLASS && !ptr[3])) {
                    Debug(1, "Mangle: '=' missing argument in '%s'\n", control);
                    return NULL;
                } else {
                    register int i;
                    if ((i = Char2Int(ptr[1])) < 0) {
                        Debug(1, "Mangle: '=' weird argument in '%s'\n", control);
                        return NULL;
                    }
                    if (ptr[2] != RULE_CLASS) {
                        ptr += 2;
                        if (area[i] != *ptr) {
                            return NULL;
                        }
                    } else {
                        ptr += 3;
                        if (!MatchClass(*ptr, area[i])) {
                            return NULL;
                        }
                    }
                }
                break;

            case RULE_DFIRST:
                if (area[0]) {
                    register int i;
                    for (i = 1; area[i]; i++) {
                        area[i - 1] = area[i];
                    }
                    area[i - 1] = '\0';
                }
                break;

            case RULE_DLAST:
                if (area[0]) {
                    register int i;
                    for (i = 1; area[i]; i++)
                        ;
                    area[i - 1] = '\0';
                }
                break;

            case RULE_MFIRST:
                if (!ptr[1] || (ptr[1] == RULE_CLASS && !ptr[2])) {
                    Debug(1, "Mangle: '(' missing argument in '%s'\n", control);
                    return NULL;
                } else {
                    if (ptr[1] != RULE_CLASS) {
                        ptr++;
                        if (area[0] != *ptr) {
                            return NULL;
                        }
                    } else {
                        ptr += 2;
                        if (!MatchClass(*ptr, area[0])) {
                            return NULL;
                        }
                    }
                }
            case RULE_MLAST:
                if (!ptr[1] || (ptr[1] == RULE_CLASS && !ptr[2])) {
                    Debug(1, "Mangle: ')' missing argument in '%s'\n", control);
                    return NULL;
                } else {
                    register int i;

                    for (i = 0; area[i]; i++)
                        ;

                    if (i > 0) {
                        i--;
                    } else {
                        return NULL;
                    }

                    if (ptr[1] != RULE_CLASS) {
                        ptr++;
                        if (area[i] != *ptr) {
                            return NULL;
                        }
                    } else {
                        ptr += 2;
                        if (!MatchClass(*ptr, area[i])) {
                            return NULL;
                        }
                    }
                }

            default:
                Debug(1, "Mangle: unknown command %c in %s\n", *ptr, control);
                return NULL;
                break;
        }
    }
    if (!area[0]) /* have we deweted de poor widdle fing away? */
    {
        return NULL;
    }
    return (area);
}

int PMatch(control, string) register char *control;
register char *string;
{
    while (*string && *control) {
        if (!MatchClass(*control, *string)) {
            return (0);
        }

        string++;
        control++;
    }

    if (*string || *control) {
        return (0);
    }

    return (1);
}

static int _PWIsBroken64(FILE *ifp) {
    PWDICT64 pdesc64;

    rewind(ifp);
    if (!fread((char *)&pdesc64.header, sizeof(pdesc64.header), 1, ifp)) {
        return 0;
    }

    return (pdesc64.header.pih_magic == PIH_MAGIC);
}

PWDICT *PWOpen(prefix, mode) const char *prefix;
char *mode;
{
    int use64 = 0;
    static PWDICT pdesc;
    static PWDICT64 pdesc64;
    char iname[STRINGSIZE];
    char dname[STRINGSIZE];
    char wname[STRINGSIZE];
    void *dfp;
    void *ifp;
    void *wfp;

    if (pdesc.header.pih_magic == PIH_MAGIC) {
        fprintf(stderr, "%s: another dictionary already open\n", prefix);
        return NULL;
    }

    memset(&pdesc, '\0', sizeof(pdesc));
    memset(&pdesc64, '\0', sizeof(pdesc64));

    snprintf(iname, STRINGSIZE, "%s.pwi", prefix);
    snprintf(dname, STRINGSIZE, "%s.pwd", prefix);
    snprintf(wname, STRINGSIZE, "%s.hwm", prefix);

    if (mode[0] == 'r') {
        pdesc.flags &= ~PFOR_USEZLIB;
        /* first try the normal db file */
        if (!(pdesc.dfp = fopen(dname, mode))) {
#ifdef HAVE_ZLIB_H
            pdesc.flags |= PFOR_USEZLIB;
            /* try extension .gz */
            snprintf(dname, STRINGSIZE, "%s.pwd.gz", prefix);
            if (!(pdesc.dfp = gzopen(dname, mode))) {
                perror(dname);
                return NULL;
            }
#else
            perror(dname);
            return NULL;
#endif
        }
    } else {
        pdesc.flags &= ~PFOR_USEZLIB;
        /* write mode: use fopen */
        if (!(pdesc.dfp = fopen(dname, mode))) {
            perror(dname);
            return NULL;
        }
    }

    if (!(pdesc.ifp = fopen(iname, mode))) {
#ifdef HAVE_ZLIB_H
        if (pdesc.flags & PFOR_USEZLIB)
            gzclose(pdesc.dfp);
        else
#endif
            fclose(pdesc.dfp);
        perror(iname);
        return NULL;
    }

    if ((pdesc.wfp = fopen(wname, mode))) {
        pdesc.flags |= PFOR_USEHWMS;
    }

    ifp = pdesc.ifp;
    dfp = pdesc.dfp;
    wfp = pdesc.wfp;

    if (mode[0] == 'w') {
        pdesc.flags |= PFOR_WRITE;
        pdesc.header.pih_magic = PIH_MAGIC;
        pdesc.header.pih_blocklen = NUMWORDS;
        pdesc.header.pih_numwords = 0;

        fwrite((char *)&pdesc.header, sizeof(pdesc.header), 1, ifp);
    } else {
        pdesc.flags &= ~PFOR_WRITE;

        if (!fread((char *)&pdesc.header, sizeof(pdesc.header), 1, ifp)) {
            fprintf(stderr, "%s: error reading header\n", prefix);

            pdesc.header.pih_magic = 0;
            fclose(ifp);
#ifdef HAVE_ZLIB_H
            if (pdesc.flags & PFOR_USEZLIB)
                gzclose(dfp);
            else
#endif
                fclose(dfp);
            if (wfp) {
                fclose(wfp);
            }
            return NULL;
        }

        if ((pdesc.header.pih_magic == 0) || (pdesc.header.pih_numwords == 0)) {
            /* uh-oh. either a broken "64-bit" file or a garbage file. */
            rewind(ifp);
            if (!fread((char *)&pdesc64.header, sizeof(pdesc64.header), 1, ifp)) {
                fprintf(stderr, "%s: error reading header\n", prefix);

                pdesc.header.pih_magic = 0;
                fclose(ifp);
#ifdef HAVE_ZLIB_H
                if (pdesc.flags & PFOR_USEZLIB)
                    gzclose(dfp);
                else
#endif
                    fclose(dfp);
                if (wfp) {
                    fclose(wfp);
                }
                return NULL;
            }
            if (pdesc64.header.pih_magic != PIH_MAGIC) {
                /* nope, not "64-bit" after all */
                fprintf(stderr, "%s: error reading header\n", prefix);

                pdesc.header.pih_magic = 0;
                fclose(ifp);
#ifdef HAVE_ZLIB_H
                if (pdesc.flags & PFOR_USEZLIB)
                    gzclose(dfp);
                else
#endif
                    fclose(dfp);

                if (wfp) {
                    fclose(wfp);
                }
                return NULL;
            }
            pdesc.header.pih_magic = pdesc64.header.pih_magic;
            pdesc.header.pih_numwords = pdesc64.header.pih_numwords;
            pdesc.header.pih_blocklen = pdesc64.header.pih_blocklen;
            pdesc.header.pih_pad = pdesc64.header.pih_pad;
            use64 = 1;
        }

        if (pdesc.header.pih_magic != PIH_MAGIC) {
            fprintf(stderr, "%s: magic mismatch\n", prefix);

            pdesc.header.pih_magic = 0;
            fclose(ifp);
#ifdef HAVE_ZLIB_H
            if (pdesc.flags & PFOR_USEZLIB)
                gzclose(dfp);
            else
#endif
                fclose(dfp);

            if (wfp) {
                fclose(wfp);
            }
            return NULL;
        }

        if (pdesc.header.pih_numwords < 1) {
            fprintf(stderr, "%s: invalid word count\n", prefix);

            pdesc.header.pih_magic = 0;
            fclose(ifp);
#ifdef HAVE_ZLIB_H
            if (pdesc.flags & PFOR_USEZLIB)
                gzclose(dfp);
            else
#endif
                fclose(dfp);
            if (wfp) {
                fclose(wfp);
            }
            return NULL;
        }

        if (pdesc.header.pih_blocklen != NUMWORDS) {
            fprintf(stderr, "%s: size mismatch\n", prefix);

            pdesc.header.pih_magic = 0;
            fclose(ifp);
#ifdef HAVE_ZLIB_H
            if (pdesc.flags & PFOR_USEZLIB)
                gzclose(dfp);
            else
#endif
                fclose(dfp);
            if (wfp) {
                fclose(wfp);
            }
            return NULL;
        }

        if (pdesc.flags & PFOR_USEHWMS) {
            int i;

            if (use64) {
                if (fread(pdesc64.hwms, 1, sizeof(pdesc64.hwms), wfp) != sizeof(pdesc64.hwms)) {
                    pdesc.flags &= ~PFOR_USEHWMS;
                }
                for (i = 0; i < sizeof(pdesc.hwms) / sizeof(pdesc.hwms[0]); i++) {
                    pdesc.hwms[i] = pdesc64.hwms[i];
                }
            } else if (fread(pdesc.hwms, 1, sizeof(pdesc.hwms), wfp) != sizeof(pdesc.hwms)) {
                pdesc.flags &= ~PFOR_USEHWMS;
            }
#if DEBUG
            for (i = 1; i <= 0xff; i++) {
                printf("hwm[%02x] = %d\n", i, pdesc.hwms[i]);
            }
#endif
        }
    }

    return (&pdesc);
}

int PWClose(pwp) PWDICT *pwp;
{
    if (pwp->header.pih_magic != PIH_MAGIC) {
        fprintf(stderr, "PWClose: close magic mismatch\n");
        return (-1);
    }

    if (pwp->flags & PFOR_WRITE) {
        pwp->flags |= PFOR_FLUSH;
        PutPW(pwp, NULL); /* flush last index if necess */

        if (fseek(pwp->ifp, 0L, 0)) {
            fprintf(stderr, "index magic fseek failed\n");
            return (-1);
        }

        if (!fwrite((char *)&pwp->header, sizeof(pwp->header), 1, pwp->ifp)) {
            fprintf(stderr, "index magic fwrite failed\n");
            return (-1);
        }

        if (pwp->flags & PFOR_USEHWMS) {
            int i;
            for (i = 1; i <= 0xff; i++) {
                if (!pwp->hwms[i]) {
                    pwp->hwms[i] = pwp->hwms[i - 1];
                }
#if DEBUG
                printf("hwm[%02x] = %d\n", i, pwp->hwms[i]);
#endif
            }
            fwrite(pwp->hwms, 1, sizeof(pwp->hwms), pwp->wfp);
        }
    }

    fclose(pwp->ifp);
#ifdef HAVE_ZLIB_H
    if (pwp->flags & PFOR_USEZLIB)
        gzclose(pwp->dfp);
    else
#endif
        fclose(pwp->dfp);
    if (pwp->wfp) {
        fclose(pwp->wfp);
    }

    pwp->header.pih_magic = 0;

    return (0);
}

int PutPW(pwp, string) PWDICT *pwp;
char *string;
{
    if (!(pwp->flags & PFOR_WRITE)) {
        return (-1);
    }

    if (string) {
        strncpy(pwp->data_put[pwp->count], string, MAXWORDLEN);
        pwp->data_put[pwp->count][MAXWORDLEN - 1] = '\0';

        pwp->hwms[string[0] & 0xff] = pwp->header.pih_numwords;

        ++(pwp->count);
        ++(pwp->header.pih_numwords);

    } else if (!(pwp->flags & PFOR_FLUSH)) {
        return (-1);
    }

    if ((pwp->flags & PFOR_FLUSH) || !(pwp->count % NUMWORDS)) {
        int i;
        uint32_t datum;
        register char *ostr;

        datum = (uint32_t)ftell(pwp->dfp);

        fwrite((char *)&datum, sizeof(datum), 1, pwp->ifp);

        fputs(pwp->data_put[0], pwp->dfp);
        putc(0, (FILE *)pwp->dfp);

        ostr = pwp->data_put[0];

        for (i = 1; i < NUMWORDS; i++) {
            register int j;
            register char *nstr;
            nstr = pwp->data_put[i];

            if (nstr[0]) {
                for (j = 0; ostr[j] && nstr[j] && (ostr[j] == nstr[j]); j++)
                    ;
                putc(j & 0xff, (FILE *)pwp->dfp);
                fputs(nstr + j, pwp->dfp);
            }
            putc(0, (FILE *)pwp->dfp);

            ostr = nstr;
        }

        memset(pwp->data_put, '\0', sizeof(pwp->data_put));
        pwp->count = 0;
    }
    return (0);
}

char *GetPW(pwp, number) PWDICT *pwp;
uint32_t number;
{
    uint32_t datum;
    register int i;
    register char *ostr;
    register char *nstr;
    register char *bptr;
    char buffer[NUMWORDS * MAXWORDLEN];
    uint32_t thisblock;

    thisblock = number / NUMWORDS;

    if (_PWIsBroken64(pwp->ifp)) {
        uint64_t datum64;
        if (fseek(pwp->ifp, sizeof(struct pi_header64) + (thisblock * sizeof(uint64_t)), 0)) {
            perror("(index fseek failed)");
            return NULL;
        }

        if (!fread((char *)&datum64, sizeof(datum64), 1, pwp->ifp)) {
            perror("(index fread failed)");
            return NULL;
        }
        datum = datum64;
    } else {
        if (fseek(pwp->ifp, sizeof(struct pi_header) + (thisblock * sizeof(uint32_t)), 0)) {
            perror("(index fseek failed)");
            return NULL;
        }

        if (!fread((char *)&datum, sizeof(datum), 1, pwp->ifp)) {
            perror("(index fread failed)");
            return NULL;
        }
    }

    int r = 1;
#ifdef HAVE_ZLIB_H
    if (pwp->flags & PFOR_USEZLIB) {
        r = gzseek(pwp->dfp, datum, 0);
        if (r >= 0)
            r = 0;
    } else
#endif
        r = fseek(pwp->dfp, datum, 0);

    if (r) {
        perror("(data fseek failed)");
        return NULL;
    }
    r = 0;

    memset(buffer, 0, sizeof(buffer));
#ifdef HAVE_ZLIB_H
    if (pwp->flags & PFOR_USEZLIB) {
        r = gzread(pwp->dfp, buffer, sizeof(buffer));
        if (r < 0)
            r = 0;
    } else
#endif
        r = fread(buffer, 1, sizeof(buffer), pwp->dfp);

    if (!r) {
        perror("(data fread failed)");
        return NULL;
    }

    bptr = buffer;

    for (ostr = pwp->data_get[0]; (*(ostr++) = *(bptr++)); /* nothing */)
        ;

    ostr = pwp->data_get[0];

    for (i = 1; i < NUMWORDS; i++) {
        nstr = pwp->data_get[i];
        strcpy(nstr, ostr);

        ostr = nstr + *(bptr++);
        while ((*(ostr++) = *(bptr++)))
            ;

        ostr = nstr;
    }

    return (pwp->data_get[number % NUMWORDS]);
}

unsigned int FindPW(pwp, string) PWDICT *pwp;
char *string;
{
    register uint32_t lwm;
    register uint32_t hwm;
    register uint32_t middle;
    register char *this;
    int idx;

#if DEBUG
    fprintf(stderr, "look for (%s)\n", string);
#endif

    if (pwp->flags & PFOR_USEHWMS) {
        idx = string[0] & 0xff;
        lwm = idx ? pwp->hwms[idx - 1] : 0;
        hwm = pwp->hwms[idx];

#if DEBUG
        fprintf(stderr, "idx = %d\n", idx);
        fprintf(stderr, "lwm = %d,  hwm = %d\n", lwm, hwm);
#endif
    } else {
        lwm = 0;
        hwm = PW_WORDS(pwp) - 1;
    }

    /* if the high water mark is lower than the low water mark, something is
     * screwed up */
    if (hwm < lwm) {
        lwm = 0;
        hwm = PW_WORDS(pwp) - 1;
    }

#if DEBUG
    fprintf(stderr, "---- %lu, %lu ----\n", lwm, hwm);
#endif

    middle = lwm + ((hwm - lwm + 1) / 2);

    for (;;) {
        int cmp;

#if DEBUG
        fprintf(stderr, "lwm = %lu,  middle = %lu,  hwm = %lu\n", lwm, middle, hwm);
#endif

        this = GetPW(pwp, middle);
        if (!this) {
#if DEBUG
            fprintf(stderr, "getpw returned null, returning null in FindPW\n");
#endif
            return (PW_WORDS(pwp));
        } else {
#if DEBUG
            fprintf(stderr, "comparing %s against found %s\n", string, this);
#endif
        }

        cmp = strcmp(string, this);
        if (cmp == 0) {
            return (middle);
        }

        if (middle == hwm) {
#if DEBUG
            fprintf(stderr, "at terminal subdivision, stopping search\n");
#endif
            break;
        }

        if (cmp < 0) {
            hwm = middle;
            middle = lwm + ((hwm - lwm) / 2);
        } else if (cmp > 0) {
            lwm = middle;
            middle = lwm + ((hwm - lwm + 1) / 2);
        }
    }

    return (PW_WORDS(pwp));
}

char *FascistLookUser(PWDICT *pwp, char *instring, const char *user, const char *gecos) {
    int i, maxrepeat;
    char *ptr;
    char *jptr;
    char junk[STRINGSIZE];
    char *password;
    char rpassword[STRINGSIZE];
    uint32_t notfound;

    notfound = PW_WORDS(pwp);
    /* already truncated if from FascistCheck() */
    /* but pretend it wasn't ... */
    strncpy(rpassword, instring, TRUNCSTRINGSIZE);
    rpassword[TRUNCSTRINGSIZE - 1] = '\0';
    password = rpassword;

    if (strlen(password) < 4) {
        return _("it is WAY too short");
    }

    if (strlen(password) < MINLEN) {
        return _("it is too short");
    }

    jptr = junk;
    *jptr = '\0';

    for (i = 0; i < STRINGSIZE && password[i]; i++) {
        if (!strchr(junk, password[i])) {
            *(jptr++) = password[i];
            *jptr = '\0';
        }
    }

    if (strlen(junk) < MINDIFF) {
        return _("it does not contain enough DIFFERENT characters");
    }

    strcpy(password, (char *)Lowercase(password));

    Trim(password);

    while (*password && isspace(*password)) {
        password++;
    }

    if (!*password) {
        return _("it is all whitespace");
    }

    i = 0;
    ptr = password;
    while (ptr[0] && ptr[1]) {
        if ((ptr[1] == (ptr[0] + 1)) || (ptr[1] == (ptr[0] - 1))) {
            i++;
        }
        ptr++;
    }

    /*  Change by Ben Karsin from ITS at University of Hawaii at Manoa.  Static
       MAXSTEP would generate many false positives for long passwords. */
    maxrepeat = 3 + (0.09 * strlen(password));
    if (i > maxrepeat) {
        return _("it is too simplistic/systematic");
    }

    if (PMatch("aadddddda", password)) /* smirk */
    {
        return _("it looks like a National Insurance number.");
    }

    /* it should be safe to use Mangle with its reliance on STRINGSIZE
       since password cannot be longer than TRUNCSTRINGSIZE;
       nonetheless this is not an elegant solution */

    for (i = 0; r_destructors[i]; i++) {
        char *a;

        if (!(a = Mangle(password, r_destructors[i]))) {
            continue;
        }

#ifdef DEBUG
        printf("%-16s (dict)\n", a);
#endif

        if (FindPW(pwp, a) != notfound) {
            return _("it is based on a dictionary word");
        }
    }

    strcpy(password, (char *)Reverse(password));

    for (i = 0; r_destructors[i]; i++) {
        char *a;

        if (!(a = Mangle(password, r_destructors[i]))) {
            continue;
        }
#ifdef DEBUG
        printf("%-16s (reversed dict)\n", a);
#endif
        if (FindPW(pwp, a) != notfound) {
            return _("it is based on a (reversed) dictionary word");
        }
    }

    return NULL;
}

char *FascistLook(pwp, instring) PWDICT *pwp;
char *instring;
{ return FascistLookUser(pwp, instring, NULL, NULL); }

const char *FascistCheckUser(password, path, user, gecos) const char *password;
const char *path;
const char *user;
const char *gecos;
{
    PWDICT *pwp;
    char pwtrunced[STRINGSIZE];
    char *res;

    /* security problem: assume we may have been given a really long
       password (buffer attack) and so truncate it to a workable size;
       try to define workable size as something from which we cannot
       extend a buffer beyond its limits in the rest of the code */

    strncpy(pwtrunced, password, TRUNCSTRINGSIZE);
    pwtrunced[TRUNCSTRINGSIZE - 1] = '\0'; /* enforce */

    /* perhaps someone should put something here to check if password
       is really long and syslog() a message denoting buffer attacks?  */

    if (!(pwp = PWOpen(path, "r"))) {
        return _("error loading dictionary");
    }

    /* sure seems like we should close the database, since we're only likely to
     * check one password */
    res = FascistLookUser(pwp, pwtrunced, user, gecos);

    PWClose(pwp);
    pwp = (PWDICT *)0;

    return res;
}

const char *FascistCheck(password, path) const char *password;
const char *path;
{ return FascistCheckUser(password, path, NULL, NULL); }

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("%s must specify the password and dictionary file path\n", argv[0]);
        return -1;
    }

    printf("%s\n", FascistCheck(argv[1], argv[2]));
    return 0;
}