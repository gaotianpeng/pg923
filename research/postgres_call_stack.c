PostgresMain(username)
{
	StringInfoData input_message;
    ReadCommand(&input_message);
    const char* query_string = pg_getmsgstring(&input_message)
    exec_simple_query(const char* query_string)
    {
        parsetree_list = pg_parse_query(query_string)
        {
            raw_parsertree_list = raw_parser(query_string)
        }

        foreach(parsetree_item, parsetree_list)
        {
            Node* parsetree = lfirst(parsetree_item)
         	querytree_list = pg_analyze_and_rewrite(parsetree, query_string, NULL, 0)
         	plantree_list = pg_plan_queries(querytree_list, 0, NULL)

         	PortalDefQuery(portal, NULL, query_string, commandTag, plantree_list, NULL)
         	PortalStart(portal, NULL, 0, InvalidSnapshot)
         	PortalSetResultFormat(portal, 1, &format)
         	receiver = CreateDestReceiver(dest)
         	PortalRun(portal, FETCH_ALL, isTopLevel, receiver, receiver, completionTag)
         	PortalDrop(portal, false)
        }
    }
}


// pg_analyze_and_rewrite ------------------------------------------------------
List *
pg_analyze_and_rewrite(Node *parsetree, const char *query_string,
					   Oid *paramTypes, int numParams)
{

	Query	   *query;
	query = parse_analyze(parsetree, query_string, paramTypes, numParams);

	querytree_list = pg_rewrite_query(query);	
}

	Query *
	parse_analyze(Node *parseTree, const char *sourceText,
				  Oid *paramTypes, int numParams)
	{
		ParseState *pstate = make_parsestate(NULL);
		Query	   *query;
		query = transformTopLevelStmt(pstate, parseTree);
		return query;
	}

	static List *
	pg_rewrite_query(Query *query)
	{
		List	   *querytree_list;
		querytree_list = QueryRewrite(query);
		return querytree_list;
	}

		List *
		QueryRewrite(Query *parsetree)
		{
			List	   *querylist;
			List	   *results;	

			queryList = RewriteQuery(parsetree, NIL);
			foreach(l, querylist)
			{
				Query	   *query = (Query *) lfirst(l);
				query = fireRIRrules(query, NIL, false);
				results = lappend(results, query);
			}

			return results;
		}

// pg_plan_queries --------------------------------------------------------------------
List *
pg_plan_queries(List *querytrees, int cursorOptions, ParamListInfo boundParams)
{
	List	   *stmt_list = NIL;
	foreach(query_list, querytrees)
	{
		Query	   *query = (Query *) lfirst(query_list);
		stmt = (Node *) pg_plan_query(query, cursorOptions, boundParams);
		stmt_list = lappend(stmt_list, stmt);
	}

	return stmt_list;
}

	PlannedStmt *
	pg_plan_query(Query *querytree, int cursorOptions, ParamListInfo boundParams)
	{
		PlannedStmt *plan;
		plan = planner(querytree, cursorOptions, boundParams);
		return plan;
	}

// PortalStart --------------------------------------------------------------------------
void
PortalStart(Portal portal, ParamListInfo params,
			int eflags, Snapshot snapshot)
{
	QueryDesc  *queryDesc;
	queryDesc = CreateQueryDesc((PlannedStmt *) linitial(portal->stmts),
							portal->sourceText,
							GetActiveSnapshot(),
							InvalidSnapshot,
							None_Receiver,
							params,
							0);
	ExecutorStart(queryDesc, myeflags);
}

// CreateDestReceiver -------------------------------------------------------------------
DestReceiver *
CreateDestReceiver(CommandDest dest)
{
	switch (dest)
	{
		case DestRemote:
		case DestRemoteExecute:
			return printtup_create_DR(dest);
	}

	/* should never get here */
	return &donothingDR;
}

// PortalRun -------------------------------------------------------------------------
bool
PortalRun(Portal portal, long count, bool isTopLevel,
		  DestReceiver *dest, DestReceiver *altdest,
		  char *completionTag)
{
	PortalRunSelect(portal, true, count, dest)
	{
		queryDesc = PortalGetQueryDesc(portal);
		ExecutorRun(queryDesc, direction, count);
	}
}

	static long
	PortalRunSelect(Portal portal,
					bool forward,
					long count,
					DestReceiver *dest)
	{
		QueryDesc  *queryDesc;
		ScanDirection direction;
		queryDesc = PortalGetQueryDesc(portal);
		ExecutorRun(queryDesc, direction, count);
	}

		void
		ExecutorRun(QueryDesc *queryDesc,
					ScanDirection direction, long count)
		{
			if (ExecutorRun_hook)
				(*ExecutorRun_hook) (queryDesc, direction, count);
			else
				standard_ExecutorRun(queryDesc, direction, count);
		}