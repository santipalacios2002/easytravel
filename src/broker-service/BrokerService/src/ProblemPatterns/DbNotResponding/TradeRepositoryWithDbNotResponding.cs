using EasyTrade.BrokerService.Helpers;
using EasyTrade.BrokerService.ProblemPatterns.OpenFeature;

namespace EasyTrade.BrokerService.Entities.Trades.Repository;

public class TradeRepositoryWithDbNotResponding : TradeRepository
{
    private readonly IPluginManager _pluginManager;

    public TradeRepositoryWithDbNotResponding(BrokerDbContext dbContext, IPluginManager pluginManager) : base(dbContext)
        => _pluginManager = pluginManager;

    public override void AddTrade(Trade trade)
    {
        if(CheckIfProblemPatternIsOn())
        {
            trade.Id = Constants.InvalidTradeId;
        }
        base.AddTrade(trade);
    }

    private bool CheckIfProblemPatternIsOn()
    {
        var task = Task.Run(async () => await _pluginManager.GetPluginState(Constants.DbNotResponding, false));
        return task.Result;
    }
}