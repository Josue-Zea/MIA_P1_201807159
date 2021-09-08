// A Bison parser, made by GNU Bison 3.5.1.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.



// First part of user prologue.
#line 18 "parser.yy"

    #include <string>
    #include <stdio.h>
    #include "driver.h"
    #include <iostream>
    void crearComandos(std::string comando , std::string argumentos);

#line 48 "parser.tab.cc"


#include "parser.tab.hh"




#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 125 "parser.tab.cc"


  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  Parser::Parser (Driver& driver_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      driver (driver_yyarg)
  {}

  Parser::~Parser ()
  {}

  Parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  Parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Parser::symbol_number_type
  Parser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[+state];
  }

  Parser::stack_symbol_type::stack_symbol_type ()
  {}

  Parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.type_get ())
    {
      case 23: // ENTERO
        value.YY_MOVE_OR_COPY< float > (YY_MOVE (that.value));
        break;

      case 24: // IDENTIFICADOR
      case 25: // IDPART
      case 26: // RUTA1
      case 27: // RUTA2
      case 28: // CADENA
      case 29: // FSVAL
      case 33: // PARAMETROS
      case 34: // PARAMETRO
      case 35: // VALORES
      case 36: // RUTAS
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  Parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.type_get ())
    {
      case 23: // ENTERO
        value.move< float > (YY_MOVE (that.value));
        break;

      case 24: // IDENTIFICADOR
      case 25: // IDPART
      case 26: // RUTA1
      case 27: // RUTA2
      case 28: // CADENA
      case 29: // FSVAL
      case 33: // PARAMETROS
      case 34: // PARAMETRO
      case 35: // VALORES
      case 36: // RUTAS
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 23: // ENTERO
        value.copy< float > (that.value);
        break;

      case 24: // IDENTIFICADOR
      case 25: // IDPART
      case 26: // RUTA1
      case 27: // RUTA2
      case 28: // CADENA
      case 29: // FSVAL
      case 33: // PARAMETROS
      case 34: // PARAMETRO
      case 35: // VALORES
      case 36: // RUTAS
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 23: // ENTERO
        value.move< float > (that.value);
        break;

      case 24: // IDENTIFICADOR
      case 25: // IDPART
      case 26: // RUTA1
      case 27: // RUTA2
      case 28: // CADENA
      case 29: // FSVAL
      case 33: // PARAMETROS
      case 34: // PARAMETRO
      case 35: // VALORES
      case 36: // RUTAS
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " (";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  void
  Parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::operator() ()
  {
    return parse ();
  }

  int
  Parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (driver));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case 23: // ENTERO
        yylhs.value.emplace< float > ();
        break;

      case 24: // IDENTIFICADOR
      case 25: // IDPART
      case 26: // RUTA1
      case 27: // RUTA2
      case 28: // CADENA
      case 29: // FSVAL
      case 33: // PARAMETROS
      case 34: // PARAMETRO
      case 35: // VALORES
      case 36: // RUTAS
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 4:
#line 53 "parser.yy"
                                    { crearComandos("mkdisk", yystack_[0].value.as < std::string > ()); }
#line 625 "parser.tab.cc"
    break;

  case 5:
#line 54 "parser.yy"
                                    { crearComandos("rmdisk", yystack_[0].value.as < std::string > ()); }
#line 631 "parser.tab.cc"
    break;

  case 6:
#line 55 "parser.yy"
                                    { crearComandos("fdisk", yystack_[0].value.as < std::string > ()); }
#line 637 "parser.tab.cc"
    break;

  case 7:
#line 56 "parser.yy"
                                    { crearComandos("pause", ""); }
#line 643 "parser.tab.cc"
    break;

  case 8:
#line 57 "parser.yy"
                                    { crearComandos("rep",yystack_[0].value.as < std::string > ()); }
#line 649 "parser.tab.cc"
    break;

  case 9:
#line 58 "parser.yy"
                                    { crearComandos("mount",yystack_[0].value.as < std::string > ()); }
#line 655 "parser.tab.cc"
    break;

  case 10:
#line 59 "parser.yy"
                                    { crearComandos("umount",yystack_[0].value.as < std::string > ()); }
#line 661 "parser.tab.cc"
    break;

  case 11:
#line 60 "parser.yy"
                                    { crearComandos("mkfs",yystack_[0].value.as < std::string > ()); }
#line 667 "parser.tab.cc"
    break;

  case 12:
#line 63 "parser.yy"
                                    { yylhs.value.as < std::string > ()=yystack_[1].value.as < std::string > ()+yystack_[0].value.as < std::string > (); }
#line 673 "parser.tab.cc"
    break;

  case 13:
#line 64 "parser.yy"
                                    { yylhs.value.as < std::string > ()=yystack_[0].value.as < std::string > (); }
#line 679 "parser.tab.cc"
    break;

  case 14:
#line 67 "parser.yy"
                                            { yylhs.value.as < std::string > ()="SIZE:"+std::to_string(yystack_[0].value.as < float > ())+"&"; }
#line 685 "parser.tab.cc"
    break;

  case 15:
#line 68 "parser.yy"
                                            { yylhs.value.as < std::string > ()="F:"+yystack_[0].value.as < std::string > ()+"&"; }
#line 691 "parser.tab.cc"
    break;

  case 16:
#line 69 "parser.yy"
                                            { yylhs.value.as < std::string > ()="U:"+yystack_[0].value.as < std::string > ()+"&"; }
#line 697 "parser.tab.cc"
    break;

  case 17:
#line 70 "parser.yy"
                                            { yylhs.value.as < std::string > ()="PATH:"+yystack_[0].value.as < std::string > ()+"&"; }
#line 703 "parser.tab.cc"
    break;

  case 18:
#line 71 "parser.yy"
                                            { yylhs.value.as < std::string > ()="TYPE:"+yystack_[0].value.as < std::string > ()+"&"; }
#line 709 "parser.tab.cc"
    break;

  case 19:
#line 72 "parser.yy"
                                            { yylhs.value.as < std::string > ()="DELETE:"+yystack_[0].value.as < std::string > ()+"&"; }
#line 715 "parser.tab.cc"
    break;

  case 20:
#line 73 "parser.yy"
                                            { yylhs.value.as < std::string > ()="NAME:"+yystack_[0].value.as < std::string > ()+"&"; }
#line 721 "parser.tab.cc"
    break;

  case 21:
#line 74 "parser.yy"
                                            { yylhs.value.as < std::string > ()="ADD:"+std::to_string(yystack_[0].value.as < float > ())+"&"; }
#line 727 "parser.tab.cc"
    break;

  case 22:
#line 75 "parser.yy"
                                            { yylhs.value.as < std::string > ()="ID:"+yystack_[0].value.as < std::string > ()+"&"; }
#line 733 "parser.tab.cc"
    break;

  case 23:
#line 76 "parser.yy"
                                            { yylhs.value.as < std::string > ()="FS:"+yystack_[0].value.as < std::string > ()+"&"; }
#line 739 "parser.tab.cc"
    break;

  case 24:
#line 79 "parser.yy"
                                            { yylhs.value.as < std::string > ()=yystack_[0].value.as < std::string > (); }
#line 745 "parser.tab.cc"
    break;

  case 25:
#line 80 "parser.yy"
                                            { yylhs.value.as < std::string > ()=yystack_[0].value.as < std::string > (); }
#line 751 "parser.tab.cc"
    break;

  case 26:
#line 83 "parser.yy"
                                            { yylhs.value.as < std::string > ()=yystack_[0].value.as < std::string > (); }
#line 757 "parser.tab.cc"
    break;

  case 27:
#line 84 "parser.yy"
                                            { yylhs.value.as < std::string > ()=yystack_[0].value.as < std::string > (); }
#line 763 "parser.tab.cc"
    break;

  case 28:
#line 85 "parser.yy"
                                            { yylhs.value.as < std::string > ()=yystack_[0].value.as < std::string > (); }
#line 769 "parser.tab.cc"
    break;

  case 29:
#line 86 "parser.yy"
                                            { yylhs.value.as < std::string > ()=yystack_[0].value.as < std::string > (); }
#line 775 "parser.tab.cc"
    break;


#line 779 "parser.tab.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yysyntax_error_ (yystack_[0].state, yyla));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[+yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yy_error_token_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yy_error_token_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    std::ptrdiff_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */
    if (!yyla.empty ())
      {
        symbol_number_type yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];

        int yyn = yypact_[+yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yy_error_token_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char Parser::yypact_ninf_ = -23;

  const signed char Parser::yytable_ninf_ = -1;

  const signed char
  Parser::yypact_[] =
  {
      11,    -4,    -4,    -4,   -23,    -4,    -4,    -4,    -4,     0,
     -23,    27,    -4,   -23,    -4,    -4,    -4,    -4,    -4,    -4,
     -23,   -23,     2,     3,     8,    13,    14,    15,    16,    19,
      28,    33,   -23,   -13,    20,   -19,    22,    23,    34,   -22,
      40,    12,    35,   -23,   -23,   -23,   -23,   -23,   -23,   -23,
     -23,   -23,   -23,   -23,   -23,   -23,   -23,   -23,   -23
  };

  const signed char
  Parser::yydefact_[] =
  {
       0,     0,     0,     0,     7,     0,     0,     0,     0,     0,
       3,     0,     4,    13,     5,     6,     8,     9,    10,    11,
       1,     2,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    12,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    14,    16,    29,    26,    28,    27,    17,
      15,    18,    19,    25,    24,    20,    21,    22,    23
  };

  const signed char
  Parser::yypgoto_[] =
  {
     -23,   -23,    56,    54,    36,   -23,   -23
  };

  const signed char
  Parser::yydefgoto_[] =
  {
      -1,     9,    10,    12,    13,    55,    49
  };

  const signed char
  Parser::yytable_[] =
  {
      20,    11,    53,     1,     2,    45,    54,    46,    47,    48,
      43,     3,    33,    34,     1,     2,     4,     5,    35,     6,
       7,     8,     3,    36,    37,    38,    39,     4,     5,    40,
       6,     7,     8,    22,    23,    24,    25,    57,    41,    26,
      27,    28,    29,    42,    44,    30,    50,    51,    32,    31,
      32,    32,    32,    32,    32,    32,    14,    15,    52,    16,
      17,    18,    19,    56,    58,    21
  };

  const signed char
  Parser::yycheck_[] =
  {
       0,     5,    24,     3,     4,    24,    28,    26,    27,    28,
      23,    11,    10,    10,     3,     4,    16,    17,    10,    19,
      20,    21,    11,    10,    10,    10,    10,    16,    17,    10,
      19,    20,    21,     6,     7,     8,     9,    25,    10,    12,
      13,    14,    15,    10,    24,    18,    24,    24,    12,    22,
      14,    15,    16,    17,    18,    19,     2,     3,    24,     5,
       6,     7,     8,    23,    29,     9
  };

  const signed char
  Parser::yystos_[] =
  {
       0,     3,     4,    11,    16,    17,    19,    20,    21,    31,
      32,     5,    33,    34,    33,    33,    33,    33,    33,    33,
       0,    32,     6,     7,     8,     9,    12,    13,    14,    15,
      18,    22,    34,    10,    10,    10,    10,    10,    10,    10,
      10,    10,    10,    23,    24,    24,    26,    27,    28,    36,
      24,    24,    24,    24,    28,    35,    23,    25,    29
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    30,    31,    31,    32,    32,    32,    32,    32,    32,
      32,    32,    33,    33,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    35,    35,    36,    36,    36,    36
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     2,     1,     2,     2,     2,     1,     2,     2,
       2,     2,     2,     1,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     1,     1,     1,     1,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "\"eof\"", "error", "$undefined", "MKDISK", "RMDISK", "MENOS", "SIZE",
  "U", "PATH", "F", "IGUAL", "FDISK", "TYPE", "DELETE", "NAME", "ADD",
  "PAUSE", "REP", "ID", "MOUNT", "UMOUNT", "MKFS", "FS", "ENTERO",
  "IDENTIFICADOR", "IDPART", "RUTA1", "RUTA2", "CADENA", "FSVAL",
  "$accept", "INSTRUCCIONES", "INSTRUCCION", "PARAMETROS", "PARAMETRO",
  "VALORES", "RUTAS", YY_NULLPTR
  };

#if YYDEBUG
  const signed char
  Parser::yyrline_[] =
  {
       0,    49,    49,    50,    53,    54,    55,    56,    57,    58,
      59,    60,    63,    64,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    79,    80,    83,    84,    85,    86
  };

  // Print the state stack on the debug stream.
  void
  Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Parser::yy_reduce_print_ (int yyrule)
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


} // yy
#line 1190 "parser.tab.cc"

#line 89 "parser.yy"


void yy::Parser::error( const std::string& error){
  std::cout << error << std::endl;
}
