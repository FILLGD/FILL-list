
exports.handler = async () => {
  const levels = [
    {
      rank: 1,
      name: "Test Level",
      creators: "Supergold99",
      points: 100
    }
  ];

  return {
    statusCode: 200,
    headers: {
      "Content-Type": "application/json",
      "Access-Control-Allow-Origin": "*"
    },
    body: JSON.stringify(levels)
  };
};
